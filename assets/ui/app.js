// PixelVerse Legends — Interactive Companion Showcase
// 60 FPS Animation, Web Audio Synthesizer & Challenge Evaluator

// 1. Web Audio Engine (Mirroring C++ SoundEngine & WavGenerator)
class WebAudioEngine {
  constructor() {
    this.ctx = null;
    this.muted = false;
  }

  init() {
    if (!this.ctx) {
      const AudioCtx = window.AudioContext || window.webkitAudioContext;
      this.ctx = new AudioCtx();
    }
  }

  playTone(freq, duration, type = 'sine', volume = 0.2) {
    if (this.muted) return;
    this.init();
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();
    osc.type = type;
    osc.frequency.setValueAtTime(freq, this.ctx.currentTime);
    
    gain.gain.setValueAtTime(volume, this.ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + duration);

    osc.connect(gain);
    gain.connect(this.ctx.destination);
    osc.start();
    osc.stop(this.ctx.currentTime + duration);
  }

  playArpeggio(notes, noteDuration = 0.12, type = 'sine', volume = 0.25) {
    if (this.muted) return;
    this.init();
    notes.forEach((freq, idx) => {
      setTimeout(() => {
        this.playTone(freq, noteDuration, type, volume);
      }, idx * (noteDuration * 1000 * 0.8));
    });
  }

  playClick() { this.playTone(800, 0.05, 'triangle', 0.15); }
  playReward() { this.playArpeggio([523.25, 659.25, 783.99, 1046.50], 0.12, 'sine', 0.3); }
  playVictory() { this.playArpeggio([392.00, 523.25, 659.25, 783.99], 0.18, 'triangle', 0.35); }
  playError() { this.playArpeggio([220.00, 164.81, 110.00], 0.1, 'sawtooth', 0.2); }
  playNotification() { this.playArpeggio([880.0, 1320.0], 0.08, 'sine', 0.25); }
  playPurchase() { this.playArpeggio([987.77, 1318.51], 0.15, 'triangle', 0.3); }
  playTournamentWin() { this.playArpeggio([523.25, 523.25, 523.25, 659.25, 783.99, 1046.50], 0.14, 'square', 0.2); }
}

const audio = new WebAudioEngine();

// 2. 60 FPS Particle Background Canvas
const canvas = document.getElementById('particleCanvas');
const ctx = canvas.getContext('2d');
let width, height;
let particles = [];

function resizeCanvas() {
  width = canvas.width = window.innerWidth;
  height = canvas.height = window.innerHeight;
}
window.addEventListener('resize', resizeCanvas);
resizeCanvas();

class Particle {
  constructor(burst = false, originX = width / 2, originY = height / 2) {
    this.burst = burst;
    this.reset(burst, originX, originY);
  }

  reset(burst = false, originX = width / 2, originY = height / 2) {
    if (burst) {
      this.x = originX;
      this.y = originY;
      const angle = Math.random() * Math.PI * 2;
      const speed = 3 + Math.random() * 8;
      this.vx = Math.cos(angle) * speed;
      this.vy = Math.sin(angle) * speed;
      this.alpha = 1.0;
      this.decay = 0.015 + Math.random() * 0.02;
      this.color = ['#FFD700', '#FF007F', '#00F0FF', '#FFFFFF'][Math.floor(Math.random() * 4)];
      this.size = 2 + Math.random() * 4;
    } else {
      this.x = Math.random() * width;
      this.y = Math.random() * height;
      this.vx = (Math.random() - 0.5) * 0.6;
      this.vy = (Math.random() - 0.5) * 0.6 - 0.3; // Gentle upward drift
      this.alpha = 0.2 + Math.random() * 0.6;
      this.size = 1 + Math.random() * 2.5;
      this.color = Math.random() > 0.5 ? '#00F0FF' : '#8A2BE2';
    }
  }

  update() {
    this.x += this.vx;
    this.y += this.vy;
    if (this.burst) {
      this.alpha -= this.decay;
      this.vy += 0.15; // Gravity
      return this.alpha > 0;
    } else {
      if (this.x < 0) this.x = width;
      if (this.x > width) this.x = 0;
      if (this.y < 0) this.y = height;
      if (this.y > height) this.y = 0;
      return true;
    }
  }

  draw(context) {
    context.save();
    context.globalAlpha = Math.max(0, this.alpha);
    context.fillStyle = this.color;
    context.shadowBlur = 8;
    context.shadowColor = this.color;
    context.beginPath();
    context.arc(this.x, this.y, this.size, 0, Math.PI * 2);
    context.fill();
    context.restore();
  }
}

// Populate background floating stars & energy embers
for (let i = 0; i < 65; ++i) {
  particles.push(new Particle(false));
}

function animateParticles() {
  ctx.clearRect(0, 0, width, height);
  for (let i = particles.length - 1; i >= 0; i--) {
    const p = particles[i];
    const alive = p.update();
    if (!alive && p.burst) {
      particles.splice(i, 1);
    } else {
      p.draw(ctx);
    }
  }
  requestAnimationFrame(animateParticles);
}
animateParticles();

function triggerBurst(x, y, count = 40) {
  for (let i = 0; i < count; i++) {
    particles.push(new Particle(true, x, y));
  }
}

// 3. UI Interactions & Toast System
function showToast(message, icon = '⚡') {
  const container = document.getElementById('toastContainer');
  const toast = document.createElement('div');
  toast.className = 'toast';
  toast.innerHTML = `<span>${icon}</span><span>${message}</span>`;
  container.appendChild(toast);
  audio.playNotification();

  setTimeout(() => {
    toast.style.opacity = '0';
    toast.style.transform = 'translateX(100%)';
    setTimeout(() => toast.remove(), 300);
  }, 3500);
}

// Navigation Tabs
document.querySelectorAll('.nav-btn').forEach(btn => {
  btn.addEventListener('click', () => {
    audio.playClick();
    document.querySelectorAll('.nav-btn').forEach(b => b.classList.remove('active'));
    document.querySelectorAll('.tab-pane').forEach(tab => tab.classList.remove('active'));
    btn.classList.add('active');
    const target = btn.getAttribute('data-tab');
    document.getElementById(`tab-${target}`).classList.add('active');
  });
});

// Sound Toggle
const btnSound = document.getElementById('btnSoundToggle');
btnSound.addEventListener('click', () => {
  audio.muted = !audio.muted;
  btnSound.textContent = audio.muted ? '🔇' : '🔊';
  showToast(audio.muted ? 'Sound Muted' : 'Sound Feedback Enabled');
});

// 4. Inventory Data & Rendering
const inventoryData = [
  { id: 101, name: "Neon Plasma Katana", category: "Weapon", rarity: "Legendary", power: 140, coins: 5000, gems: 150, equipped: true },
  { id: 102, name: "Cyber Void Bow", category: "Weapon", rarity: "Epic", power: 95, coins: 3200, gems: 80, equipped: false },
  { id: 103, name: "Quantum Hyper-Revolver", category: "Weapon", rarity: "Rare", power: 70, coins: 1800, gems: 30, equipped: false },
  { id: 201, name: "Chrono Valkyrie Outfit", category: "Skin", rarity: "Mythic", power: 0, coins: 12000, gems: 400, equipped: true },
  { id: 202, name: "Glitch Shadow Robes", category: "Skin", rarity: "Epic", power: 0, coins: 4500, gems: 120, equipped: false },
  { id: 301, name: "Mecha-Dragon Pet", category: "Pet", rarity: "Mythic", power: 50, coins: 20000, gems: 600, equipped: true },
  { id: 401, name: "Overdrive XP Booster", category: "PowerUp", rarity: "Rare", power: 0, coins: 800, gems: 15, equipped: false }
];

function renderInventory(filter = 'all') {
  const grid = document.getElementById('inventoryGrid');
  grid.innerHTML = '';
  const filtered = filter === 'all' ? inventoryData : inventoryData.filter(i => i.category === filter);

  filtered.forEach(item => {
    const card = document.createElement('div');
    card.className = `item-card ${item.rarity.toLowerCase()}`;
    card.innerHTML = `
      ${item.equipped ? '<span class="equipped-badge">EQUIPPED</span>' : ''}
      <div>
        <span class="rarity-pill ${item.rarity}">${item.rarity}</span>
        <h3 class="item-name">${item.name}</h3>
        <div class="item-power">PWR: +${item.power}</div>
      </div>
      <div style="display:flex; justify-content:space-between; align-items:center; margin-top:12px;">
        <span class="mono-num">${item.coins.toLocaleString()} 🪙</span>
        <button class="btn-neon secondary" style="padding:6px 12px; font-size:12px;">${item.equipped ? 'Unequip' : 'Equip'}</button>
      </div>
    `;
    card.querySelector('button').addEventListener('click', () => {
      audio.playClick();
      item.equipped = !item.equipped;
      renderInventory(filter);
      showToast(`${item.name} ${item.equipped ? 'Equipped' : 'Unequipped'}`);
    });
    grid.appendChild(card);
  });
}
renderInventory();

document.querySelectorAll('.filter-btn').forEach(btn => {
  btn.addEventListener('click', () => {
    audio.playClick();
    document.querySelectorAll('.filter-btn').forEach(b => b.classList.remove('active'));
    btn.classList.add('active');
    renderInventory(btn.getAttribute('data-cat'));
  });
});

// 5. Leaderboard Rendering
const leaderboardData = [
  { rank: 1, name: "QuantumTitan", tier: "Master", score: 6100, wins: 580, streak: 15 },
  { rank: 2, name: "PixelMage", tier: "Diamond", score: 5200, wins: 410, streak: 12 },
  { rank: 3, name: "CyberValkyrie", tier: "Diamond", score: 4850, wins: 310, streak: 8 },
  { rank: 4, name: "GlitchStalker", tier: "Diamond", score: 4850, wins: 290, streak: 5 },
  { rank: 5, name: "ShadowBlade", tier: "Gold", score: 3900, wins: 220, streak: 3 },
  { rank: 6, name: "NeonRanger", tier: "Silver", score: 2800, wins: 160, streak: 2 }
];

function renderLeaderboard() {
  const tbody = document.querySelector('#leaderboardTable tbody');
  tbody.innerHTML = '';
  leaderboardData.forEach(p => {
    const tr = document.createElement('tr');
    tr.innerHTML = `
      <td><strong style="color:var(--neon-cyan)">#${p.rank}</strong></td>
      <td><strong>${p.name}</strong></td>
      <td><span class="rarity-pill Rare">${p.tier}</span></td>
      <td><span class="mono-num">${p.score.toLocaleString()}</span></td>
      <td>${p.wins}</td>
      <td><span style="color:var(--neon-pink)">${p.streak} 🔥</span></td>
    `;
    tbody.appendChild(tr);
  });
}
renderLeaderboard();

// 6. Tournament Pagination
let currentPage = 1;
const pageSize = 4;
const totalMatches = 16;

function renderTournamentMatches(page) {
  const list = document.getElementById('tournamentMatches');
  list.innerHTML = '';
  const startIndex = (page - 1) * pageSize;
  const endIndex = Math.min(startIndex + pageSize, totalMatches);

  for (let i = startIndex; i < endIndex; i++) {
    const matchId = i + 1;
    const stage = matchId <= 8 ? "Round of 16" : (matchId <= 12 ? "Quarter-Final" : (matchId <= 14 ? "Semi-Final" : "Grand Final"));
    const p1 = `Gladiator_${matchId * 2 - 1}`;
    const p2 = `Gladiator_${matchId * 2}`;
    const winner = matchId % 2 === 0 ? p1 : p2;

    const card = document.createElement('div');
    card.className = 'match-card';
    card.innerHTML = `
      <div class="match-stage">STAGE: ${stage.toUpperCase()} (MATCH #${matchId})</div>
      <div class="match-vs">
        <span style="${winner === p1 ? 'color:var(--neon-gold)' : ''}">${p1}</span>
        <span style="font-size:12px; color:var(--text-secondary)">VS</span>
        <span style="${winner === p2 ? 'color:var(--neon-gold)' : ''}">${p2}</span>
      </div>
      <div style="margin-top:10px; font-size:12px;">Winner: <span class="match-winner">${winner}</span> 🏆</div>
    `;
    list.appendChild(card);
  }

  document.getElementById('pageIndicator').textContent = `Page ${page} / 4 (Matches ${startIndex + 1}-${endIndex})`;
}
renderTournamentMatches(currentPage);

document.getElementById('btnPrevPage').addEventListener('click', () => {
  audio.playClick();
  if (currentPage > 1) {
    currentPage--;
    renderTournamentMatches(currentPage);
  }
});
document.getElementById('btnNextPage').addEventListener('click', () => {
  audio.playClick();
  if (currentPage < 4) {
    currentPage++;
    renderTournamentMatches(currentPage);
  }
});

// 7. Interactive Reward Chest Opening
const btnOpenChest = document.getElementById('btnOpenChest');
const chestBox = document.getElementById('chestBox');
let chestOpened = false;

function openChestAction() {
  if (chestOpened) return;
  chestOpened = true;
  audio.playReward();

  // Burst particles
  const rect = chestBox.getBoundingClientRect();
  triggerBurst(rect.left + rect.width / 2, rect.top + rect.height / 2, 70);

  chestBox.style.transform = 'scale(1.25) rotate(4deg)';
  setTimeout(() => { chestBox.style.transform = 'scale(1.0)'; }, 300);

  document.getElementById('rewardTitle').innerHTML = '<span style="color:var(--neon-gold)">REWARDS CLAIMED!</span>';
  document.getElementById('rewardSubtitle').textContent = '+1,000 Gold Coins, +50 Hyper-Gems, Neon Plasma Katana Upgrade unlocked!';
  btnOpenChest.textContent = 'Claimed ✓';
  btnOpenChest.classList.remove('pulse');
  btnOpenChest.disabled = true;

  // Animate HUD Coins
  const hudCoins = document.getElementById('hudCoins');
  hudCoins.textContent = '46,000';
  document.getElementById('hudGems').textContent = '1,250';
  showToast('Reward Unlocked: +1,000 Coins, +50 Gems!', '🎁');
}
btnOpenChest.addEventListener('click', openChestAction);
chestBox.addEventListener('click', openChestAction);

// 8. Shop & History Demo Data
const shopItems = [
  { name: "Void-Walker Exosuit", price: "25,000 Coins", rarity: "Mythic", desc: "Teleport dash animation and cosmic trails." },
  { name: "Neon Hyper-Blaster", price: "8,500 Coins", rarity: "Legendary", desc: "Dual energy projectiles with plasma impact." },
  { name: "Chrono Mecha-Wolf", price: "350 Gems", rarity: "Mythic", desc: "Cyber companion providing +10% trophy boost." }
];
const shopGrid = document.getElementById('shopGrid');
shopItems.forEach(s => {
  const div = document.createElement('div');
  div.className = 'item-card';
  div.innerHTML = `
    <span class="rarity-pill ${s.rarity}">${s.rarity}</span>
    <h3 class="item-name">${s.name}</h3>
    <p style="font-size:13px; color:var(--text-secondary); margin:8px 0;">${s.desc}</p>
    <div style="display:flex; justify-content:space-between; align-items:center; margin-top:12px;">
      <span class="mono-num">${s.price}</span>
      <button class="btn-neon primary" style="padding:6px 14px; font-size:12px;">Purchase</button>
    </div>
  `;
  div.querySelector('button').addEventListener('click', () => {
    audio.playPurchase();
    showToast(`Purchased ${s.name}!`, '💎');
  });
  shopGrid.appendChild(div);
});

const historyData = [
  { outcome: "Victory", vs: "ShadowBlade", delta: "+35 Trophies", score: "3 - 1", time: "12m ago" },
  { outcome: "Victory", vs: "Gladiator_7", delta: "+28 Trophies", score: "3 - 0", time: "1h ago" },
  { outcome: "Victory", vs: "PixelMage", delta: "+45 Trophies", score: "3 - 2", time: "3h ago" },
  { outcome: "Defeat", vs: "QuantumTitan", delta: "-18 Trophies", score: "2 - 3", time: "5h ago" }
];
const historyList = document.getElementById('historyList');
historyData.forEach(h => {
  const row = document.createElement('div');
  row.className = 'match-card';
  row.style.marginBottom = '10px';
  const isVic = h.outcome === 'Victory';
  row.innerHTML = `
    <div style="display:flex; justify-content:space-between; align-items:center;">
      <div style="display:flex; align-items:center; gap:12px;">
        <span class="rarity-pill ${isVic ? 'Legendary' : 'Mythic'}">${h.outcome}</span>
        <strong>VS ${h.vs}</strong>
        <span style="font-size:12px; color:var(--text-secondary)">Score: ${h.score}</span>
      </div>
      <div>
        <span class="mono-num" style="color:${isVic ? 'var(--neon-green)' : 'var(--neon-pink)'}">${h.delta}</span>
        <span style="font-size:11px; color:var(--text-secondary); margin-left:10px;">${h.time}</span>
      </div>
    </div>
  `;
  historyList.appendChild(row);
});

// 9. Debugging Challenge Inspector & Section 12 Output
const challengeBugs = [
  { id: 1, title: "Bug 1: Coin Filter Boundary Bug", concept: "Boundary Condition (< vs <=)", symptom: "Coin filter excludes maximum value (excludes players where coins == maxCoins)." },
  { id: 2, title: "Bug 2: Tournament Pagination Bug", concept: "0-Based Indexing / Off-by-One", symptom: "Every tournament page skips one match on offset calculation ((page - 1) * pageSize + 1)." },
  { id: 3, title: "Bug 3: Leaderboard Sorting Bug", concept: "Comparator Direction (< vs >)", symptom: "Leaderboard ranks players with lowest score at the top (Ascending instead of Descending)." },
  { id: 4, title: "Bug 4: Reward Queue Bug", concept: "Queue Ordering (push_front vs push_back)", symptom: "Priority VIP rewards appear after normal rewards in claim queue." }
];

const bugsContainer = document.getElementById('challengeBugsList');
challengeBugs.forEach(b => {
  const card = document.createElement('div');
  card.className = 'bug-card';
  card.innerHTML = `
    <div class="bug-header">
      <span class="bug-title">${b.title}</span>
      <span class="bug-concept">${b.concept}</span>
    </div>
    <div class="bug-symptom"><strong>QA Symptom:</strong> ${b.symptom}</div>
  `;
  bugsContainer.appendChild(card);
});

const jsonOutputArea = document.getElementById('jsonOutputArea');
const sampleJsonChallenge = {
  "Bug 1: Coin Filter Boundary": {"Status": "failed", "Execution time": "3ms"},
  "Bug 2: Tournament Pagination": {"Status": "failed", "Execution time": "2ms"},
  "Bug 3: Leaderboard Sorting": {"Status": "failed", "Execution time": "4ms"},
  "Bug 4: Reward Queue": {"Status": "failed", "Execution time": "2ms"},
  "Total bugs": 4,
  "Passed": 0,
  "Failed": 4,
  "Total Execution time": "11ms"
};

const sampleJsonFixed = {
  "Bug 1: Coin Filter Boundary": {"Status": "passed", "Execution time": "3ms"},
  "Bug 2: Tournament Pagination": {"Status": "passed", "Execution time": "1ms"},
  "Bug 3: Leaderboard Sorting": {"Status": "passed", "Execution time": "4ms"},
  "Bug 4: Reward Queue": {"Status": "passed", "Execution time": "2ms"},
  "Total bugs": 4,
  "Passed": 4,
  "Failed": 0,
  "Total Execution time": "10ms"
};

jsonOutputArea.textContent = JSON.stringify(sampleJsonChallenge, null, 2);

let fixedToggle = false;
document.getElementById('btnRunTests').addEventListener('click', () => {
  fixedToggle = !fixedToggle;
  if (fixedToggle) {
    audio.playVictory();
    document.getElementById('evalModeBadge').textContent = "FIX_BUGS Mode (All 36 Passed)";
    document.getElementById('evalModeBadge').style.color = "var(--neon-green)";
    jsonOutputArea.textContent = JSON.stringify(sampleJsonFixed, null, 2);
    showToast("BUILD SUCCESS: All 4 bugs verified fixed!", "🏆");
  } else {
    audio.playError();
    document.getElementById('evalModeBadge').textContent = "Challenge Mode (4 Bugs Active)";
    document.getElementById('evalModeBadge').style.color = "var(--neon-pink)";
    jsonOutputArea.textContent = JSON.stringify(sampleJsonChallenge, null, 2);
    showToast("Challenge Mode: 4 bugs active (deterministic QA failure state).", "⚠️");
  }
});
