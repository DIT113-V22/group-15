
// Allows our website to use speech to text.
function runSpeechRecognition() {
    // get output div reference
    var output = document.getElementById("output");
    // get action element reference
    var action = document.getElementById("action");
    // new speech recognition object
    var SpeechRecognition = SpeechRecognition || webkitSpeechRecognition;
    var recognition = new SpeechRecognition();

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
        var transcript = event.results[0][0].transcript;
        output.innerHTML = "<b>You said:</b> " + transcript;
        output.classList.remove("hide");

        document.getElementById('searchbar').value = transcript;

        search_item()

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
function search_item() {
    let input = document.getElementById('searchbar').value
    input = input.toLowerCase();
    let x = document.querySelector('#result');
    x.innerHTML = ""

    for (i = 0; i < itemData.length; i++) {
        let obj = itemData[i];

        if (obj.Item.toLowerCase().includes(input)) {
            const elem = document.createElement("li")
            elem.innerHTML = `${obj.Item} - Quantity: ${obj.Quantity} - Price: ${obj.Price} | Location:  Aisle: ${obj.Aisle} - Section: ${obj.Section} `
            x.appendChild(elem)
        }

    }
}

//--------------- Predefined path --------
/* //not in use anymore, scope changed.
function addRows(){
    const table = document.getElementById('myTable');
    const rowCount = table.rows.length;
    const cellCount = table.rows[0].cells.length;
    const row = table.insertRow(rowCount);

    for(var i =0; i <= cellCount; i++){
        let cell = 'cell'+i;
        cell = row.insertCell(i);
        cell.innerHTML=document.getElementById('column' + i).innerHTML;


    }
}
// not in use

function deleteRows(){
    const table = document.getElementById('myTable');
    let rowCount = table.rows.length;
    if(rowCount > '2'){
        const row = table.deleteRow(rowCount-1);
        rowCount--;
    }
    else{
        alert('There should be at least one stage');
    }
}

// not in use
function submit(){
        const speed = Number.parseInt(document.getElementById("speed").value, 10);
        const distance = Number.parseInt(document.getElementById("distance").value, 10);
        const degrees = Number.parseInt(document.getElementById("degrees").value, 10);

        const data = {speed, distance, degrees};

    postData('http://localhost:', data)
        .then(response => {
            console.log(response);
        });

}
// not in use

async function postData(url = '', data = {}) {
    const response = await fetch(url, {
        method: 'POST',
        mode: 'no-cors',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    });
    return response.json();
} */