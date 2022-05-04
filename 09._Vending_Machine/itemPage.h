const char itemPage_html[] PROGMEM = R"rawliteral(
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
        section {
            max-width: 420px;
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
        <h1 class="display-3 text-center mb-3">Vending Machine</h1>
        <hr>
        <div class="container">
            <div class="row justify-content-between">
                <p class="col-6">Selected Item:</p>
                <strong class="col-6" id="item-name"></strong>
            </div>
        </div>
        <div class="container">
            <div class="row justify-content-between">
                <p class="col-6">Price:</p>
                <strong class="col-6" id="item-price"></strong>
            </div>
        </div>    
        <div id="status-container" class="alert alert-light text-center">
            <p id="status"></p>
            <a href="http://192.168.1.7">
              <button style="display: none;" id="return-button" class="btn btn-sm btn-dark mt-3">Return to Home Page 🏠</button>
            </a>
        </div>
    </section>
    <script>
        const items = {
            '1': {
                price: 7000,
                emoji: '🥛'
            },
            '2': {
                price: 5000,
                emoji: '🥤'
            },
            '3': {
                price: 4000,
                emoji: '🥜'
            },
            '4': {
                price: 6000,
                emoji: '🍟'
            },
        };
        const resonseInfo = {
            INVALID: {
                message: 'Invalid Request! Please Try Again.',
                type: 'warning '
            },
            WAITING_FOR_TAG: {
                message: 'Please place your tag in front of the sensor',
                type: 'light '
            },
            AUTHENTICATING: {
                message: 'Authenticating...',
                type: 'info '
            },
            AUTHENTICATION_FAILED: {
                message: 'Authentication Failed! Please Try Again.',
                type: 'danger'
            },
            CHECKING_WALLET: {
                message: 'Checking Account Balance...',
                type: 'info'
            },
            NOT_ENOUGH_MONEY: {
                message: 'Your account balance is not enough.',
                type: 'danger'
            },
            OPENNING_DOOR: {
                message: 'ْThe door is openning!',
                type: 'info '
            },
            TAKING: {
                message: 'Please take your item!',
                type: 'success'
            },
            DONE: {
                message: 'Your purchase was successful. Return to the main page to buy more.',
                type: 'success'
            },
            
        }
    
        const params = new URLSearchParams(window.location.search);
        console.log(params.get('item'));
        const itemNameSection = document.getElementById('item-name');
        const itemPriceSection = document.getElementById('item-price');
        const statusSection = document.getElementById("status");
        const statusSectionContainer = document.getElementById("status-container");
        const returnButton = document.getElementById("return-button");
        const itemName = params.get('item');
        itemNameSection.innerText = itemName ? `${itemName || '?'} (${items[itemName]?.emoji})` : '-';
        itemPriceSection.innerText = `${items[itemName]?.price || '-'}`;

        setInterval(function update(){
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    var response = xhttp.responseText;
                    statusSection.innerHTML = resonseInfo[response]?.message || 'Something went wrong! Please try again';
                    statusSectionContainer.className = `alert alert-${resonseInfo[response]?.type || 'danger'} text-center`;
                    if ([
                    'AUTHENTICATING',
                    'CHECKING_WALLET',
                    'OPENNING_DOOR',
                    'TAKING'
                    ].includes(response)) {
                        returnButton.style.display = 'none';
                    } else {
                        returnButton.style.display = 'inline';
                    }
                }
            };
            xhttp.open("GET", "status", true);
            xhttp.send();
        }, 200);
    </script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>
  </body>
</html>
)rawliteral";
