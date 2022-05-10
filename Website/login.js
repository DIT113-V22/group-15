
function getInfo(){
    var username = document.getElementById("username").value;
    var password = document.getElementById("password").value;

        if((username == "admin") && password=="123"){
            window.location = "MovementControlWeb.html";
        }else {
            alert("Invalid username or password");
        }

    }