# PixelVerse Legends — AAA Multiplayer Arcade Game Engine

## 1. Application Overview
**PixelVerse Legends** is a production-grade, highly modular C++17 game engine and debugging challenge designed to simulate real-world AAA game development and QA automation. Built around a Cyber Fantasy / Neon Universe theme, the application combines a robust custom animation engine, procedural graphics, and deterministic state persistence.

### Key Capabilities
- **Player Roster Management**: Dynamic filtering of players across coin balances and ranks.
- **Tournament Match History**: Paginated fetching of historical tournament matches and brackets.
- **Global Leaderboard**: Real-time sorting and ranking of players based on match scores.
- **Reward Dispatch System**: Multi-tier priority queue dispatching VIP and daily login rewards.
- **Interactive API & UI**: An embedded HTTP dashboard to interact with core engine subsystems and observe state changes.

### Technology Stack
- **Backend**: C++17, cpp-httplib (Embedded HTTP Web Server)
- **Database**: In-memory local state (Vectors, Maps, Deques) with deterministic seed data
- **Frontend**: HTML5, Vanilla JS, CSS3 Glassmorphism UI
- **Testing**: GoogleTest (GTest), CMake, CTest

---

## 2. Debugging Challenge

This repository contains **4 realistic logical bugs** reported by QA automation and beta testers. These issues involve core Data Structures and Algorithms (DSA) concepts.

### Bug 1 — Coin Filter Boundary Bug
- **User Symptom**: QA reports that when filtering players by coin count, players holding exactly the maximum coin boundary value are omitted from the roster result list.
- **Module**: Player Dashboard / Filtering

### Bug 2 — Tournament Pagination Bug
- **User Symptom**: QA reports that navigating through tournament match history pages skips one match on each page transition (e.g., match #5 is skipped between page 1 and page 2).
- **Module**: Tournament Manager / Pagination

### Bug 3 — Leaderboard Ranking Bug
- **User Symptom**: QA reports that the leaderboard displays rookie players with low scores at Rank #1 while master players with the highest score are pushed down to the bottom.
- **Module**: Leaderboard / Sorting

### Bug 4 — Reward Queue Bug
- **User Symptom**: QA reports that high-priority VIP rewards appear after low-tier daily login rewards in the player reward queue, delaying VIP rewards until standard rewards are claimed.
- **Module**: Reward Manager / Queue Ordering

---

## 3. Expected Behavior After Fixes
Once all 4 bugs are diagnosed and resolved:
1. The coin range filter includes players holding exactly the upper boundary value.
2. Pagination displays matches contiguously without skipping items across pages.
3. The leaderboard displays master players with the highest scores at Rank #1 (descending order).
4. High-priority VIP rewards are dispatched and claimed before standard daily login rewards in the queue.

