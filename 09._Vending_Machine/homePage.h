const char homePage_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">
    <title>Vending Machine</title>
    <style>
        body {
            width: 100vw;
            height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
    </style>
  </head>
  <body>
    <section>
        <h1 class="display-3 text-center mb-3">Vending Machine</h1>
        <hr>
        <form id="vending-machine" action="/order" method="get">
            <div class="form-check">
                <input class="form-check-input" type="radio" name="item" id="milk" value="1" onchange="getIsSubmittionAvailable()">
                <label class="form-check-label" for="milk">
                    🥛<b>Milk</b> (7000)
                </label>
            </div>
            <div class="form-check mt-3">
                <input class="form-check-input" type="radio" name="item" id="soda" value="2" onchange="getIsSubmittionAvailable()">
                <label class="form-check-label" for="soda">
                    🥤<b>Soda</b> (5000)
                </label>
            </div>
            <div class="form-check mt-3">
                <input class="form-check-input" type="radio" name="item" id="peanut" value="3" onchange="getIsSubmittionAvailable()">
                <label class="form-check-label" for="peanut">
                    🥜<b>Peanut Pack</b> (4000)
                </label>
            </div>
            <div class="form-check mt-3">
                <input class="form-check-input" type="radio" name="item" id="chips" value="4" onchange="getIsSubmittionAvailable()">
                <label class="form-check-label" for="chips">
                    🍟<b>Chips</b> (6000)
                </label>
            </div>
            
        </form>
        <div class="container mt-3">
          <div class="row justify-content-between">
              <button id="order-button" value="order" onclick="order()" type="submit" class="btn btn-primary col-12" disabled>Submit Order</button>
          </div>
      </div>

    </section>
    <script>
        const getIsSubmittionAvailable = () => {
            let selectedItem = null;
            if (document.getElementById('milk').checked) {
                selectedItem = 1;
            } else if (document.getElementById('soda').checked) {
                selectedItem = 2;
            } else if (document.getElementById('peanut').checked) {
                selectedItem = 3;
            } else if (document.getElementById('chips').checked) {
                selectedItem = 4;
            }
            if (selectedItem) {
                document.getElementById("order-button").disabled = false;
            } else {
                document.getElementById("order-button").disabled = true;
            }
        }
        const order = () => {
            console.log('submitting an order...');
            let selectedItem = null;
            if (document.getElementById('milk').checked) {
                selectedItem = 1;
            } else if (document.getElementById('soda').checked) {
                selectedItem = 2;
            } else if (document.getElementById('peanut').checked) {
                selectedItem = 3;
            } else if (document.getElementById('chips').checked) {
                selectedItem = 4;
            }
            if (selectedItem) {
              var xhttp = new XMLHttpRequest();
              console.log("order?item="+selectedItem)
              xhttp.open("GET", "order?item="+selectedItem, false);
              xhttp.send();
              window.location.replace(`/order?item=${selectedItem}`);
            } else {
                alert("Please select an Item!")
            }
        }
    </script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>
  </body>
</html>
)rawliteral";
