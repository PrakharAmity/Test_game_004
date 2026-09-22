#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#define WIN32_LEAN_AND_MEAN
#endif

#include "engine/GameEngine.hpp"
#include "utils/Logger.hpp"
#include "utils/JsonHelper.hpp"
#include "utils/httplib.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

const char* HTML_CONTENT = R"html(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>PixelVerse Legends Debugging Dashboard</title>
<style>
  body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #0f0f15; color: #00ffcc; margin: 0; padding: 20px; }
  h1 { text-align: center; color: #ff00ff; text-transform: uppercase; letter-spacing: 2px; text-shadow: 0 0 10px #ff00ff; }
  .grid { display: grid; grid-template-columns: 1fr 1fr; gap: 20px; max-width: 1200px; margin: 0 auto; }
  .card { background: #1a1a24; border: 1px solid #333; padding: 20px; border-radius: 8px; box-shadow: 0 0 15px rgba(0,255,204,0.1); }
  .card h2 { margin-top: 0; color: #00ffcc; border-bottom: 1px solid #333; padding-bottom: 10px; }
  button { background: #ff00ff; color: white; border: none; padding: 10px 15px; border-radius: 5px; cursor: pointer; font-weight: bold; margin-top: 10px; }
  button:hover { background: #cc00cc; }
  pre { background: #000; padding: 10px; border-radius: 5px; overflow-x: auto; color: #00ffcc; border: 1px solid #333; min-height: 50px; }
</style>
</head>
<body>
  <h1>PixelVerse Legends Dashboard</h1>
  <div class="grid">
    <div class="card">
      <h2>Bug 1: Coin Filter Boundary</h2>
      <p>Filter players having between 1000 and 3000 coins.</p>
      <button onclick="fetch('api/players/filter?min=1000&max=3000').then(r=>r.json()).then(d=>document.getElementById('b1-res').innerText=JSON.stringify(d,null,2))">Run Filter</button>
      <pre id="b1-res">Result will appear here...</pre>
    </div>
    <div class="card">
      <h2>Bug 2: Tournament Pagination</h2>
      <p>Fetch match history page 2 (size 4).</p>
      <button onclick="fetch('api/tournament/matches?page=2&size=4').then(r=>r.json()).then(d=>document.getElementById('b2-res').innerText=JSON.stringify(d,null,2))">Fetch Page 2</button>
      <pre id="b2-res">Result will appear here...</pre>
    </div>
    <div class="card">
      <h2>Bug 3: Leaderboard Sorting</h2>
      <p>Fetch top ranked players.</p>
      <button onclick="fetch('api/leaderboard').then(r=>r.json()).then(d=>document.getElementById('b3-res').innerText=JSON.stringify(d,null,2))">Load Leaderboard</button>
      <pre id="b3-res">Result will appear here...</pre>
    </div>
    <div class="card">
      <h2>Bug 4: Reward Queue</h2>
      <p>Claim the next reward from the queue.</p>
      <button onclick="fetch('api/rewards/claim', {method:'POST'}).then(r=>r.json()).then(d=>document.getElementById('b4-res').innerText=JSON.stringify(d,null,2))">Claim Reward</button>
      <pre id="b4-res">Result will appear here...</pre>
    </div>
  </div>
</body>
</html>
)html";

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    pixelverse::engine::GameEngine engine;
    engine.initialize();
    
    // Seed test data for bugs
    engine.getPlayerDashboard().clearRoster();
    engine.getPlayerDashboard().addRosterPlayer(pixelverse::player::Player(1, "Rookie_A", "av1", 1, 0, 1000));
    engine.getPlayerDashboard().addRosterPlayer(pixelverse::player::Player(2, "Mid_B", "av2", 1, 0, 2000));
    engine.getPlayerDashboard().addRosterPlayer(pixelverse::player::Player(3, "Pro_C", "av3", 1, 0, 3000));
    
    engine.getTournamentManager().clearMatches();
    for (int i=1; i<=10; ++i) {
        engine.getTournamentManager().addMatch(pixelverse::tournament::Match(i, "P1", "P2", 2, 1, "Bracket", pixelverse::tournament::MatchStatus::Completed, "P1"));
    }
    
    engine.getRewardManager().clearQueue();
    engine.getRewardManager().enqueueReward(pixelverse::reward::Reward(1, "Daily Login", pixelverse::reward::RewardType::DailyLogin, 100, 5, 20, "", false));
    engine.getRewardManager().enqueueReward(pixelverse::reward::Reward(2, "VIP Special", pixelverse::reward::RewardType::VIPSpecial, 500, 50, 200, "", true));

    httplib::Server svr;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(HTML_CONTENT, "text/html");
    });

    svr.Get("/api/players/filter", [&](const httplib::Request& req, httplib::Response& res) {
        int min = req.has_param("min") ? std::stoi(req.get_param_value("min")) : 0;
        int max = req.has_param("max") ? std::stoi(req.get_param_value("max")) : 10000;
        auto players = engine.getPlayerDashboard().filterByCoins(min, max);
        
        pixelverse::utils::JsonValue root;
        root.type = pixelverse::utils::JsonValue::Type::Array;
        for (const auto& p : players) {
            pixelverse::utils::JsonValue pv;
            pv.type = pixelverse::utils::JsonValue::Type::Object;
            pv["id"] = p.id;
            pv["username"] = p.username;
            pv["coins"] = p.coins;
            root.arrayValue.push_back(pv);
        }
        res.set_content(root.serialize(), "application/json");
    });

    svr.Get("/api/tournament/matches", [&](const httplib::Request& req, httplib::Response& res) {
        int page = req.has_param("page") ? std::stoi(req.get_param_value("page")) : 1;
        int size = req.has_param("size") ? std::stoi(req.get_param_value("size")) : 4;
        auto matches = engine.getTournamentManager().getMatchesPage(page, size);
        
        pixelverse::utils::JsonValue root;
        root.type = pixelverse::utils::JsonValue::Type::Array;
        for (const auto& m : matches) {
            pixelverse::utils::JsonValue mv;
            mv.type = pixelverse::utils::JsonValue::Type::Object;
            mv["id"] = m.id;
            mv["winner"] = m.winnerId;
            root.arrayValue.push_back(mv);
        }
        res.set_content(root.serialize(), "application/json");
    });

    svr.Get("/api/leaderboard", [&](const httplib::Request&, httplib::Response& res) {
        auto ranked = engine.getLeaderboard().getRankedPlayers();
        pixelverse::utils::JsonValue root;
        root.type = pixelverse::utils::JsonValue::Type::Array;
        for (const auto& p : ranked) {
            pixelverse::utils::JsonValue pv;
            pv.type = pixelverse::utils::JsonValue::Type::Object;
            pv["username"] = p.username;
            pv["score"] = p.score;
            pv["rank"] = p.rank;
            root.arrayValue.push_back(pv);
        }
        res.set_content(root.serialize(), "application/json");
    });

    svr.Post("/api/rewards/claim", [&](const httplib::Request&, httplib::Response& res) {
        pixelverse::reward::Reward claimed;
        bool ok = engine.getRewardManager().claimNextReward(claimed);
        
        pixelverse::utils::JsonValue root;
        root.type = pixelverse::utils::JsonValue::Type::Object;
        if (ok) {
            root["status"] = "success";
            root["rewardId"] = claimed.id;
            root["name"] = claimed.title;
            root["isPriority"] = claimed.isPriority;
        } else {
            root["status"] = "empty";
        }
        res.set_content(root.serialize(), "application/json");
    });

    pixelverse::utils::Logger::info("Starting PixelVerse Legends server on 0.0.0.0:8080");
    svr.listen("0.0.0.0", 8080);
    return 0;
}
