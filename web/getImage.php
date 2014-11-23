<?php

include_once('config.php');

$files = glob(GET_IMG_DIR . "*.png");

do {
    $file = array_pop($files);
}
while( is_dir($file) );
do {
    $file = array_pop($files);
}
while( is_dir($file) );

// open the file in a binary mode
$fp = fopen($file, 'rb');

// send the right headers
header("Content-Type: image/png");
header("Content-Length: " . filesize($file));

// dump the picture and stop the script
fpassthru($fp);
exit;