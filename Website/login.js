
function getInfo(){
    var username = document.getElementById("username").value;
    var password = document.getElementById("password").value;
    let message = "Invalid username or password"
        if((username === "admin") && password==="123"){
            window.location = "MovementControlWeb.html";
        }else {
            alert(message);
        }

    }