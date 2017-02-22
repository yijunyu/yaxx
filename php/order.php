<?php
class Order{
    private $items = array();
    private $amount = 0;
 
    public function addItem($reference, $quantity){
        $this->items[] = array($reference, $quantity);
        $this->amount += $quantity*Catalog::getPrice($reference);
    }
 
    public function getAmount(){
        return $this->amount;
    }
}
class Catalog{
    private static $priceList = array('Largo Winch' => 9.31, 'Astérix' => 8.46, 'XIII' => 8.70);
 
    public static function getPrice($reference){
        return self::$priceList[$reference];
    }
}
$myOrder = new Order;
$myOrder->addItem('Largo Winch',2);
$myOrder->addItem('Astérix',2);
?>
