const char webpage_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html lang="en">
 <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">
    <title>Parking Door</title>
    <style>
        body {
            width: 100vw;
            height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        section {
          width: 350px;
        }
        @media screen and (max-width: 450px) {
            section {
                max-width: 90vw;
            }
        }
    </style>
  </head>
<body>
  <section>
    <h1 class="display-3 text-center mb-3">Parking Door</h1>
    <hr>
    <div class="container mb-3">
        <div class="row justify-content-between">
            <button class="btn btn-success col-5" onclick="processButton(event)" id="open">OPEN</button>
            <button class="btn btn-danger col-5" onclick="processButton(event)" id="close">CLOSE</button>
        </div>
    </div>
    
    <div id="status-container" class="alert alert-light text-center">
          <p id="status"></p>
    </div>
  </section>
</body>
<script>
const resonseInfo = {
  DOOR_IS_CLOSED: {
      message: 'Door is Closed.',
      type: 'info'
  },
  DOOR_IS_OPEN: {
      message: 'Door is Open.',
      type: 'info '
  },
  OPENING_DOOR: {
      message: 'Opening the Door...',
      type: 'warning '
  },
  CLOSING_DOOR: {
      message: 'Closing the Door...',
      type: 'warning'
  },
  OBJECT_DETECTED: {
      message: 'An object was detected! Re-Opning the door...',
      type: 'danger'
  },
}
function processButton(e){
  var target = e.target || window.target;
  console.log("sending request", target);
  let buttonState = target.id;
  var xhttp = new XMLHttpRequest();
  
  xhttp.open("GET", "button?state="+buttonState, false);
  xhttp.send();
}

setInterval(() => {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.status == 200) {
      var response = xhttp.responseText;
      document.getElementById("status").innerHTML = resonseInfo[response]?.message || 'Something went wrong! Please try again';
      document.getElementById("status-container").className = `alert alert-${resonseInfo[response]?.type || 'danger'} text-center`;
    }
  };
  xhttp.open("GET", "alert");
  xhttp.send();
}, 200);
</script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>

</html>
)rawliteral";
