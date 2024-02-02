document.addEventListener('DOMContentLoaded', function () {
    // Fecha de inicio (cambia a la fecha que desees)
    const fechaInicio = new Date('2023-07-02T00:00:00');
  
    function actualizarTiempo() {
      const ahora = new Date();
      const diferencia = ahora - fechaInicio;
  
      const meses = Math.floor(diferencia / (1000 * 60 * 60 * 24 * 30));
      const semanas = Math.floor(diferencia / (1000 * 60 * 60 * 24 * 7));
      const dias = Math.floor(diferencia / (1000 * 60 * 60 * 24));
      const horas = Math.floor(diferencia / (1000 * 60 * 60));
      const minutos = Math.floor(diferencia / (1000 * 60));
      const segundos = Math.floor(diferencia / 1000);
  
      document.getElementById('tiempo-transcurrido').innerHTML = `${meses} meses, ${semanas} semanas, ${dias} días, ${horas} horas, ${minutos} minutos, ${segundos} segundos`;
    }
  
    actualizarTiempo();
  
    // Actualiza el tiempo cada segundo
    setInterval(actualizarTiempo, 1000);

    
  // Función para crear corazones dinámicamente
  function crearCorazon() {
    const corazon = document.createElement('div');
    corazon.className = 'corazon';
    corazon.style.left = `${Math.random() * window.innerWidth}px`;
    document.getElementById('corazones-container').appendChild(corazon);
    
    // Eliminar el corazón después de la animación
    corazon.addEventListener('animationend', () => {
      corazon.remove();
    });
  }

  // Crear corazones cada cierto intervalo
  setInterval(crearCorazon, 2000);
});
