const socket = new WebSocket('ws://10.1.224.111'); 
socket.onopen = function(event) {
  console.log('WebSocket connecté');
  requestAccelerometerData();
  setInterval(requestAccelerometerData, 5000);
};
function requestAccelerometerData() {
  socket.send('getAccelerometer');
}
socket.onmessage = function(event) {
  const data = event.data.split(',');
  document.getElementById('x-axis').textContent = data[0];
  document.getElementById('y-axis').textContent = data[1];
  document.getElementById('z-axis').textContent = data[2];
};