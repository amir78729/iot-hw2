const char webpage_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">
    <title>Alarm</title>
    <style>
        body {
            width: 100vw;
            height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        section {
          min-width: 250px;
        }
    </style>
  </head>
  <body>
    <section>
        <h1 class="display-3 text-center mb-3">Alarm</h1>
        <hr>
        <form>
            <div class="mb-3">
              <label for="time" class="form-label">Time</label>
              <input type="time" class="form-control" id="time" aria-describedby="time" onchange="changeTime()">
            </div>
            <div class="mb-3">
              <label for="volume" class="form-label">Volume: <strong id="vol-num">5</strong> </label>
              <input type="range" class="form-range" min="0" max="10" step="1" id="volume" onchange="changeVolume()">
            </div>
          </form>
          <div class="container">
            <div class="row justify-content-between">
                <button id="start-btn" onclick="toggleStart()" type="submit" class="btn btn-danger col-6">Stop</button>
                <button id="snooz-btn" onclick="snooz()" type="submit" class="btn btn-primary col-6">Snooz</button>
            </div>
        </div>
    </section>
    <script>
    const startStopBtn = document.getElementById('start-btn');
    const volNum = document.getElementById('vol-num');
    const rangeInput = document.getElementById('volume');
    const timeInput = document.getElementById('time');
    const params = new URLSearchParams(window.location.search);
    
    let isStart = false;
    let volume = 5;
    let state = "stop";

    timeInput.value = params.get('time');

    volume = Number(params.get('volume'));
    rangeInput.value = volume;
    volNum.innerText = volume;

    const updateUrl = () => {
      if (history.pushState) {
          const newurl = `${window.location.protocol}//${window.location.host}${window.location.pathname}?time=${getEpoch() || 'undefined'}&volume=${volume}&state=${state}`;
          window.history.pushState({path:newurl},'',newurl);
      }
    }
    const getEpoch = () => Number(new Date(new Date().toString().substring(0, 16) + timeInput.value + ":00" + new Date().toString().substring(24)));

    const toggleStart = (e) => {
      state = isStart ? "stop": "start"
      isStart = !isStart;
      updateUrl();
      sendStopAlarm();
    }

    const snooz = () => {
      state = 'snooze';
      updateUrl();
      sendSnoozeAlarm();
    }

    function sendUpdateAlarm() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", `update_time?time=${getEpoch()}`, false);
      xhttp.send();
    }
    function sendUpdateVolume() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", `update_volume?volume=${volume}`, false);
      xhttp.send();
    }
    function sendSnoozeAlarm() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", `snooze_alarm`, false);
      xhttp.send();
    }
    function sendStopAlarm() {
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", `stop_alarm`, false);
      xhttp.send();
    }
    const changeVolume = () => {
      volume = rangeInput.value;
      volNum.innerText = volume;
      updateUrl();
      sendUpdateVolume();
    }
    const changeTime = () => {
      time = timeInput.value;
      updateUrl();
      sendUpdateAlarm();
    }
    </script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>
  </body>
</html>
)rawliteral";
