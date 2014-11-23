<?php

include_once('config.php');

$files = glob(GET_TXT_DIR . "*.txt");

do {
    $file = array_pop($files);
}
while( is_dir($file) );

do {
    $file = array_pop($files);
}
while( is_dir($file) );


if ( ($text = file_get_contents($file)) !== false ) {
    echo htmlentities($text, ENT_QUOTES | ENT_SUBSTITUTE);
}