
function getInfo(){
    var username = document.getElementById("username").value;
    var password = document.getElementById("password").value;

        if((username == "abc") && password=="123"){
            window.location = "test3.html";
        }else {
            alert("Invalid username or password");
        }

    }