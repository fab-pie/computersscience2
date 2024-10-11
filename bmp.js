const socket = new WebSocket('ws://10.1.224.111'); 
socket.onopen = function(event) {
  console.log('WebSocket connecté');
  requestBMP280Data();
  setInterval(requestBMP280Data, 5000);
};
function requestBMP280Data() {
  socket.send('getBMP280');
}
socket.onmessage = function(event) {
  const data = event.data.split(',');
  document.getElementById('temperature').textContent = data[0];
  document.getElementById('pressure').textContent = data[1];
  document.getElementById('altitude').textContent = data[2];
};