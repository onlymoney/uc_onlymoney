<?php
/**
 * omMerch Class.
 *
 * @author  YarYs
 * @project OnlyMoney
 * API_Gate_0.0.1x
 */
     class omMerch
     {
         private $serverURL;
         private $merchId;
         private $merchSign;
         
         public function __construct($merch_id, $sign, $serverURL) {
            $this->merchId = $merch_id;
            $this->merchSign = $sign;
            $this->serverURL = $serverURL;
         }
         
         private function sendRequest($aSend)
         {
           $aRequest = array();
           $aRequest['operation'] = json_encode($aSend);
           $aRequest['signature'] = base64_encode(urlencode(sha1($this->merchSign.$aRequest['operation'].$this->merchSign)));
            
           $operation = $aRequest['signature'].'/'.base64_encode(urlencode($aRequest['operation'])).'/';
           
           if($res = file($this->serverURL.$operation))
           {
             if(is_array($res) && count($res) == 1)
             {
               if(preg_match('/^[A-Za-z0-9=]+(:)[A-Za-z0-9=]+$/', $res[0]))
               {
                 $aRes = explode(':', $res[0]);
                 $signature = base64_decode($aRes[0]);
                 $operation = base64_decode($aRes[1]);
                 if( $signature && $operation )
                 {
                   return array($signature, $operation);
                 }    
               }
             }
           }
           return;
         }

         private function getResponse($aResponse)
         { 
           if(!is_array($aResponse))
           {
             return array(array('error'=>'OnlyMoney API Server не отвечает.'));
           }
                   
           if(sha1($this->merchSign.$aResponse[1].$this->merchSign) == $aResponse[0])
           {
             return json_decode($aResponse[1],1);
           }
           else 
           {
             return array(array('error'=>'Signature is invalid'));
           }
           return false;
         }
         
         public function getApiList()
         {
            return $this->getResponse($this->sendRequest(array('merchant_id'=>$this->merchId, 'action'=>'api_list')));
         }
         
         public function getCurrencies()
         {
            return $this->getResponse($this->sendRequest(array('merchant_id'=>$this->merchId, 'action'=>'currencies')));
         }
         
         public function getHistory($filtr = array())
         {
            return $this->getResponse($this->sendRequest(array('merchant_id'=>$this->merchId, 'action'=>'history', 'filtr'=>$filtr)));
         }
         
         public function getCurrencyExchange()
         {
            return $this->getResponse($this->sendRequest(array('merchant_id'=>$this->merchId, 'action'=>'currency_exchange')));
         }
         
         public function getCurrencyBalance($currency_id)
         {
            return $this->getResponse($this->sendRequest(array('merchant_id'=>$this->merchId, 'action'=>'currency_balance', 'currency_id'=>$currency_id)));
         }
         
         public function getPurseBalance($purse_id)
         {
            return $this->getResponse($this->sendRequest(array('merchant_id'=>$this->merchId, 'action'=>'purse_balance', 'purse_id'=>$purse_id)));
         }
         
         public function payAction($currency, $amount, $recipient, $comment, $action='pay_email')
         {
            return $this->getResponse($this->sendRequest(array('merchant_id'=>$this->merchId, 'action'=>$action, 'currency'=>$currency, 'amount'=>$amount, 'recipient'=>$recipient, 'comment'=>$comment)));
         }    
     }