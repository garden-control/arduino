<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $servername = "localhost";
    // REPLACE with your Database name
    $dbname = "id21218477_gardeningcontrol";
    // REPLACE with Database user
    $username = "id21218477_igorb";
    // REPLACE with Database user password
    $password = "HostMysql12!";
    
    $query = $_POST["query"];
    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }
    
    if ($conn->query($query) === true) {
        echo "Atualizado com sucesso!";
    } 
    else {
        echo "Erro: " . $query . "<br>" . $conn->error;
    }

    $conn->close();
    
}
else {
    http_response_code(400);
    die("Erro: metodo de requisicao http diferente de \"POST\"");
}