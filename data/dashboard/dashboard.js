let ws, ledOn = false;

window.addEventListener('DOMContentLoaded', () => {
  // 1) Khởi WebSocket
  const host = window.location.hostname;
  ws = new WebSocket(`ws://${host}/ws`);

  ws.onopen = () => console.log('✅ WS connected');
  ws.onclose = () => console.log('⚠️ WS closed');
  ws.onerror = e => console.error('❌ WS error', e);
  ws.onmessage = ({ data }) => {
    // Cập nhật sensor, fix 2 decimal places
    try {
      const d = JSON.parse(data);
      if (d.temperature  !== undefined) document.getElementById('temp').textContent  = parseFloat(d.temperature).toFixed(2);
      if (d.humidity     !== undefined) document.getElementById('humi').textContent  = parseFloat(d.humidity).toFixed(2);
      if (d.light        !== undefined) document.getElementById('light').textContent = parseFloat(d.light).toFixed(2);
      if (d.soil         !== undefined) document.getElementById('soil').textContent  = parseFloat(d.soil).toFixed(2);
      if (d.distance     !== undefined) document.getElementById('dist').textContent  = parseFloat(d.distance).toFixed(2);
    } catch (e) {
      console.error('WS sensor parse error', e);
    }
  };

  // 2) Thiết lập controls
  const mode = document.getElementById('rgbMode');
  const manual = document.getElementById('manualControls');
  mode.addEventListener('change', () => {
    manual.classList.toggle('hidden', mode.value !== 'RGB_MANUAL');
  });

  document.getElementById('applyRgb').addEventListener('click', () => {
    if (ws.readyState !== WebSocket.OPEN) return alert('WebSocket not connected');
    const msg = { command: 'setRgb', mode: mode.value };
    if (mode.value === 'RGB_MANUAL') {
      msg.color      = document.getElementById('colorPicker').value;
      msg.brightness = document.getElementById('brightness').value;
    }
    console.log('➡️ send', msg);
    ws.send(JSON.stringify(msg));
  });

  // 3) LED toggle
  const btn = document.getElementById('ledToggle');
  btn.addEventListener('click', () => {
    if (ws.readyState !== WebSocket.OPEN) return alert('WebSocket not connected');
    ledOn = !ledOn;
    ws.send(JSON.stringify({ command: 'toggleLed', state: ledOn ? 'ON' : 'OFF' }));
    btn.textContent = ledOn ? 'Turn Off' : 'Turn On';
    btn.classList.toggle('off', !ledOn);
  });
});
