const connectButton = document.getElementById('connect-button');
const sendButton = document.getElementById('send-button');
const messageInput = document.getElementById('message-input');
const outputDiv = document.getElementById('output');

let socket = null;

connectButton.addEventListener('click', () => {
  const domainName = '10.1.224.111';
  socket = new WebSocket(`ws://${domainName}:80`);

  socket.onopen = () => {
    console.log('Connexion établie');
    outputDiv.innerHTML += 'Connected. Redirect to home page...<br>';
    setTimeout(() => {
      window.location.href = 'home.html'; 
    }, 2000);
  };

  socket.onmessage = (event) => {
    console.log(`Message reçu : ${event.data}`);
    outputDiv.innerHTML += `Message reçu : ${event.data}<br>`;
};

  socket.onclose = () => {
    console.log('Connexion fermée');
    outputDiv.innerHTML += 'Connexion fermée<br>';
  };

  socket.onerror = (error) => {
    console.log(`Erreur : ${error}`);
    outputDiv.innerHTML += `Erreur : ${error}<br>`;
  };
});

sendButton.addEventListener('click', () => {
  const message = messageInput.value;
  socket.send(message);
  messageInput.value = '';
});
