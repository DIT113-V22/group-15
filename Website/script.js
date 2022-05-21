
// Allows our website to use speech to text. @Carl
function runSpeechRecognition() {
    // get output div reference
    let output = document.getElementById("output");
    // get action element reference
    let action = document.getElementById("action");
    // new speech recognition object
    let SpeechRecognition = SpeechRecognition || webkitSpeechRecognition;
    let recognition = new SpeechRecognition();

    // This runs when the speech recognition service starts
    recognition.onstart = function () {
        action.innerHTML = "<small>listening...</small>";
    };

    recognition.onspeechend = function () {
        //action.innerHTML = "<small>Voice result</small>";
        recognition.stop();
    }

    // This runs when the speech recognition service returns result
    recognition.onresult = function (event) {
        let transcript = event.results[0][0].transcript;
        output.innerHTML = "<b>You said:</b> " + transcript;
        output.classList.remove("hide");

        document.getElementById('searchbar').value = transcript;

        searchItem()

    };

    // start recognition
    recognition.start();
}

//---------------
//DummyData for items. @Carl
let itemJson = `[
  {"Item": "Tomato", "Quantity": "30", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Ice Cream", "Quantity": "40", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Salmon", "Quantity": "40", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Apple", "Quantity": "100", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Frozen Pizza", "Quantity": "14", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Coca-Cola", "Quantity": "14", "Price": "19 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Coca-Cola", "Quantity": "14", "Price": "12 SEK", "Aisle": "2", "Section": "B"}
]`

let itemData = JSON.parse(itemJson)

//This function is used in both voice recognition and manual search
function searchItem() {
    let input = document.getElementById('searchbar').value
    input = input.toLowerCase();
    let x = document.querySelector('#result');
    x.innerHTML = ""

    for (let i = 0; i < itemData.length; i++) {
        let obj = itemData[i];

        if (obj.Item.toLowerCase().includes(input)) {
            const elem = document.createElement("li")
            elem.innerHTML = `${obj.Item} - Quantity: ${obj.Quantity} - Price: ${obj.Price} | Location:  Aisle: ${obj.Aisle} - Section: ${obj.Section} `
            x.appendChild(elem)
        }

    }
}
