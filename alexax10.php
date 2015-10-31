<?php

function wordsToNumber($data) {
    // Replace all number words with an equivalent numeric value
    $data = strtr(
        $data,
        array(
            'zero'      => '0',
            'a'         => '1',
            'one'       => '1',
            'two'       => '2',
            'to'       => '2', // Hack
            'three'     => '3',
            'four'      => '4',
            'five'      => '5',
            'six'       => '6',
            'seven'     => '7',
            'eight'     => '8',
            'nine'      => '9',
            'ten'       => '10',
            'eleven'    => '11',
            'twelve'    => '12',
            'thirteen'  => '13',
            'fourteen'  => '14',
            'fifteen'   => '15',
            'sixteen'   => '16',
            'seventeen' => '17',
            'eighteen'  => '18',
            'nineteen'  => '19',
            'twenty'    => '20',
            'thirty'    => '30',
            'forty'     => '40',
            'fourty'    => '40', // common misspelling
            'fifty'     => '50',
            'sixty'     => '60',
            'seventy'   => '70',
            'eighty'    => '80',
            'ninety'    => '90',
            'hundred'   => '100',
            'thousand'  => '1000',
            'million'   => '1000000',
            'billion'   => '1000000000',
            'and'       => '',
        )
    );

    // Coerce all tokens to numbers
    $parts = array_map(
        function ($val) {
            return floatval($val);
        },
        preg_split('/[\s-]+/', $data)
    );

    $stack = new SplStack; // Current work stack
    $sum   = 0; // Running total
    $last  = null;

    foreach ($parts as $part) {
        if (!$stack->isEmpty()) {
            // We're part way through a phrase
            if ($stack->top() > $part) {
                // Decreasing step, e.g. from hundreds to ones
                if ($last >= 1000) {
                    // If we drop from more than 1000 then we've finished the phrase
                    $sum += $stack->pop();
                    // This is the first element of a new phrase
                    $stack->push($part);
                } else {
                    // Drop down from less than 1000, just addition
                    // e.g. "seventy one" -> "70 1" -> "70 + 1"
                    $stack->push($stack->pop() + $part);
                }
            } else {
                // Increasing step, e.g ones to hundreds
                $stack->push($stack->pop() * $part);
            }
        } else {
            // This is the first element of a new phrase
            $stack->push($part);
        }

        // Store the last processed part
        $last = $part;
    }

    return $sum + $stack->pop();
}

date_default_timezone_set('America/New_York');

$Alexa = file_get_contents('php://input');
// $Alexa = print_r($_POST,true);

   file_put_contents("/echo/alexa.log",date(DATE_ATOM) . " " . $Alexa . "\n",FILE_APPEND);
   
   $jAlexa = json_decode($Alexa,true) ;
   file_put_contents("/echo/alexa.log",date(DATE_ATOM) . " " . print_r($jAlexa,true). "\n\n",FILE_APPEND);
   
   
   
if (!empty($Alexa))
{
   $reply = "";
   $type = $jAlexa["request"]["type"];
   file_put_contents("/echo/alexa.log",date(DATE_ATOM) . " " . "I think the intent is " . $intent . "\n",FILE_APPEND);
   if ($type = "IntentRequest") 
   {
    $type = $jAlexa["request"]["intent"]["name"];
    $slots = $jAlexa["request"]["intent"]["slots"];
    $onoff = $slots["onoff"]["value"];
    $device = $slots["device"]["value"];
    $housecode = "a";
    $cmd = $housecode . wordsToNumber($device) . " " . $onoff ;
    file_put_contents("/echo/alexa.cmd",$cmd);   
    file_put_contents("/echo/alexa.log",date(DATE_ATOM) . " Sent " . $cmd . "\n",FILE_APPEND);
    $arrReply = array();
    
   $arrReply["version"] = "1.0";
   $arrReply["response"] = array("outputSpeech" => array("type" => "PlainText","text" => $cmd));
   $arrReply["shouldEndSession"] =  true;
   
   file_put_contents("/echo/alexa.log",date(DATE_ATOM) . " Sent " . print_r($arrReply,true) . "\n",FILE_APPEND);
   $reply = json_encode($arrReply);
      file_put_contents("/echo/alexa.log",date(DATE_ATOM) . " Sent " . $reply . "\n",FILE_APPEND);
   }
    
   
}
else
{
     file_put_contents("/echo/alexa.in","Hmmm --- didn't get a post");
}

echo  $reply
?>

