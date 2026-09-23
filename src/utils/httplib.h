//
//  httplib.h — Lightweight, zero-dependency embedded HTTP micro-server
//  Designed for low-resource environments (instant build, minimal RAM, cross-platform)
//

#ifndef PIXELVERSE_LIGHTWEIGHT_HTTPLIB_H
#define PIXELVERSE_LIGHTWEIGHT_HTTPLIB_H

#include <string>
#include <map>
#include <functional>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdlib>

#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #ifdef _MSC_VER
    #pragma comment(lib, "ws2_32.lib")
  #endif
  typedef SOCKET socket_t;
  #define CLOSE_SOCKET(s) closesocket(s)
  #define IS_INVALID_SOCKET(s) ((s) == INVALID_SOCKET)
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  typedef int socket_t;
  #define CLOSE_SOCKET(s) ::close(s)
  #define IS_INVALID_SOCKET(s) ((s) < 0)
#endif

namespace httplib {

struct Request {
    std::string method;
    std::string path;
    std::map<std::string, std::string> params;
    std::map<std::string, std::string> headers;
    std::string body;

    bool has_param(const std::string& key) const {
        return params.find(key) != params.end();
    }

    std::string get_param_value(const std::string& key) const {
        auto it = params.find(key);
        return (it != params.end()) ? it->second : "";
    }
};

struct Response {
    int status = 200;
    std::string body;
    std::string content_type = "text/plain";

    void set_content(const std::string& s, const std::string& ct) {
        body = s;
        content_type = ct;
    }
};

class Server {
public:
    using Handler = std::function<void(const Request&, Response&)>;

    Server() {
#ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    }

    ~Server() {
#ifdef _WIN32
        WSACleanup();
#endif
    }

    void Get(const std::string& pattern, Handler handler) {
        get_handlers[pattern] = handler;
    }

    void Post(const std::string& pattern, Handler handler) {
        post_handlers[pattern] = handler;
    }

    bool listen(const std::string& host, int port) {
        socket_t server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (IS_INVALID_SOCKET(server_fd)) return false;

        int opt = 1;
#ifdef _WIN32
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

        sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(static_cast<uint16_t>(port));
        if (host == "0.0.0.0" || host.empty()) {
            addr.sin_addr.s_addr = INADDR_ANY;
        } else {
            unsigned long ip = inet_addr(host.c_str());
            addr.sin_addr.s_addr = (ip != INADDR_NONE) ? ip : INADDR_ANY;
        }

        if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
            CLOSE_SOCKET(server_fd);
            return false;
        }

        if (::listen(server_fd, 10) != 0) {
            CLOSE_SOCKET(server_fd);
            return false;
        }

        while (true) {
            sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            socket_t client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
            if (IS_INVALID_SOCKET(client_fd)) continue;

            handle_client(client_fd);
            CLOSE_SOCKET(client_fd);
        }

        CLOSE_SOCKET(server_fd);
        return true;
    }

private:
    std::map<std::string, Handler> get_handlers;
    std::map<std::string, Handler> post_handlers;

    void handle_client(socket_t client_fd) {
        char buffer[4096];
        int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) return;
        buffer[bytes_read] = '\0';

        std::string raw(buffer, bytes_read);
        std::istringstream stream(raw);
        std::string method, full_url, proto;
        if (!(stream >> method >> full_url >> proto)) return;

        Request req;
        req.method = method;

        // Parse path and query params
        size_t qpos = full_url.find('?');
        if (qpos != std::string::npos) {
            req.path = full_url.substr(0, qpos);
            std::string query = full_url.substr(qpos + 1);
            std::istringstream qstream(query);
            std::string pair;
            while (std::getline(qstream, pair, '&')) {
                size_t eq = pair.find('=');
                if (eq != std::string::npos) {
                    req.params[pair.substr(0, eq)] = pair.substr(eq + 1);
                } else if (!pair.empty()) {
                    req.params[pair] = "";
                }
            }
        } else {
            req.path = full_url;
        }

        // Read headers and body
        std::string line;
        size_t content_length = 0;
        std::getline(stream, line); // consume remainder of request line
        while (std::getline(stream, line) && line != "\r" && !line.empty()) {
            size_t colon = line.find(':');
            if (colon != std::string::npos) {
                std::string header_name = line.substr(0, colon);
                std::string header_val = line.substr(colon + 1);
                while (!header_val.empty() && (header_val.front() == ' ' || header_val.front() == '\t')) header_val.erase(0, 1);
                while (!header_val.empty() && (header_val.back() == '\r' || header_val.back() == ' ')) header_val.pop_back();
                req.headers[header_name] = header_val;
                if (header_name == "Content-Length" || header_name == "content-length") {
                    content_length = std::stoul(header_val);
                }
            }
        }

        // Extract body
        size_t body_pos = raw.find("\r\n\r\n");
        if (body_pos != std::string::npos) {
            req.body = raw.substr(body_pos + 4);
        }

        Response res;
        bool handled = false;
        if (method == "GET" && get_handlers.find(req.path) != get_handlers.end()) {
            get_handlers[req.path](req, res);
            handled = true;
        } else if (method == "POST" && post_handlers.find(req.path) != post_handlers.end()) {
            post_handlers[req.path](req, res);
            handled = true;
        }

        if (!handled) {
            res.status = 404;
            res.set_content("404 Not Found", "text/plain");
        }

        std::string status_text = (res.status == 200) ? "200 OK" : ((res.status == 404) ? "404 Not Found" : std::to_string(res.status));
        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 " << status_text << "\r\n"
                        << "Content-Type: " << res.content_type << "\r\n"
                        << "Content-Length: " << res.body.size() << "\r\n"
                        << "Connection: close\r\n"
                        << "Access-Control-Allow-Origin: *\r\n\r\n"
                        << res.body;

        std::string resp_str = response_stream.str();
        send(client_fd, resp_str.c_str(), static_cast<int>(resp_str.size()), 0);
    }
};

} // namespace httplib

#endif // PIXELVERSE_LIGHTWEIGHT_HTTPLIB_H
