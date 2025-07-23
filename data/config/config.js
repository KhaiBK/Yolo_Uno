document.getElementById("configForm").addEventListener("submit", async function(e) {
  e.preventDefault();

  const form = new FormData(e.target);
  const cfg = {};
  form.forEach((value, key) => { cfg[key] = value; });

  const res = await fetch("/save-config", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(cfg)
  });

  if (res.ok) {
    alert("✅ Config saved. ESP will restart...");
  } else {
    alert("❌ Failed to save config.");
  }
});
