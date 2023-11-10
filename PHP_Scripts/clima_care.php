<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $query = $_POST["query"];
    $pdo;
    try {
        $pdo = new PDO("mysql:host=localhost;dbname=id21218477_gardeningcontrol", "id21218477_igorb", "HostMysql12!");
        $pdo->SetAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    }
    catch (PDOException $e) {
        http_response_code(500);
        echo "Falha ao conectar ao banco de dados: " . $e->getMessage();
        die();
    }
    try {
        $statement = $pdo->prepare($query);
        $statement->execute();
        $result = $statement->fetchAll(PDO::FETCH_ASSOC);
        if (empty($result)) {
            http_response_code(200);
            echo "Nenhum erro";
            die();
        }
        else {
            http_response_code(200);
            echo "Nenhum erro\n";
            print_r($result);
            die();
        }
    }
    catch (PDOException $e) {
        http_response_code(400);
        echo "Erro: " . $e->getMessage();
        die();
    }
}
else {
    http_response_code(400);
    die("Erro: metodo de requisicao http diferente de \"POST\"");
}