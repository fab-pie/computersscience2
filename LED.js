const socket = new WebSocket('ws://10.1.224.111'); 
const ledControl = document.getElementById('ledControl'); 
const ledIntensity = document.getElementById('ledIntensity');

ledControl.addEventListener('input', function() {
  const intensity = ledControl.value; 
  ledIntensity.textContent = intensity; 
  socket.send('setLED:' + intensity);
});

socket.onopen = function(event) {
  console.log('WebSocket connecté');
};

socket.onmessage = function(event) {
  console.log('Message reçu du serveur: ', event.data); 
};
