<?php
/*class dht11{
 public $link='';
 function __construct($identifier){
  $this->connect();
  $this->storeInDB($identifier);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'prueba_arduino') or die('Cannot select the DB');
 }
 
 function storeInDB($identifier){
  $query = "insert into asistencia set codigo='".$identifier."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}
if($_GET['identifier'] != ''){
 $dht11=new dht11($_GET['identifier']);
}*/
class dht11{
 public $link='';
 function __construct($identifier, $classroom){
  $this->connect();
  $this->storeInDB($identifier, $classroom);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'prueba_arduino') or die('Cannot select the DB');
 }
 
 function storeInDB($identifier, $classroom){
  $query = "INSERT into asistencia(codigo, Aula)  VALUES('$identifier', '$classroom')";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}
if($_GET['identifier'] != '' and  $_GET['classroom'] != ''){
 $dht11=new dht11($_GET['identifier'],$_GET['classroom']);
}


?>
