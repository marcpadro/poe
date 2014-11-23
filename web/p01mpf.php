<!DOCTYPE html>

<html>
<head>
    <meta charset="utf-8">
    <title>Through Proxy 0.0.0.1</title>
    <link type="text/css" rel="stylesheet" href="p01mpf.css" />
</head>

<body>
    
    <div id='p01mpf'>
        <input type='checkbox' id='p01mpf_close' style='display:none;' />
        <input type='checkbox' id='p01mpf_more' style='display:none;' />
        <div id='p01mpf_main'>
            <label for='p01mpf_close'>X</label>
            <div id='p01mpf_h'>
                <span class='through'>Through</span>
                <a class='proxy' href='/' target='_parent'>Proxy 0.0.0.1</a>
            </div>
            <label for='p01mpf_more' id='p01mpf_more_off'>+</label>
            <label for='p01mpf_more' id='p01mpf_more_on'>-</label>
            <div id='p01mpf_more_content'>
                <img id='p01mpf_image' src="getImage.php" width="75px" height="100px" />
                <div id="p01mpf_text"><?php include 'getText.php'; ?></div>
            </div>
            
        </div>
    </div>
    
</body>
<script type='text/javascript' src='http://code.jquery.com/jquery-1.10.2.min.js' ></script>
<script type='text/javascript'>
    var interval = null;
    
    $( document ).ready( function(){
        $('#p01mpf_more').change(function(){
            if (this.checked) {
                interval = setInterval( function(){
        
                    $('#p01mpf_image').attr('src', "getImage.php?"+Math.random());
                    
                    $('#p01mpf_text').load('getText.php');
    
                }, 2000);
            } else if (interval !== null) {
                clearInterval(interval);
                interval = null;
            }
        });
        
        $('#p01mpf_close').change(function(){
            
            if (interval !== null) {
                clearInterval(interval);
                interval = null;
            }
        });
        
        $('#p01mpf_main').hover(function(){
            if (document.getElementById('p01mpf_more').checked) {
                interval = setInterval( function(){
        
                    $('#p01mpf_image').attr('src', "getImage.php?"+Math.random());
                    
                    $('#p01mpf_text').load('getText.php');
    
                }, 2000);
            }
        }, function(){
            if (interval !== null) {
                clearInterval(interval);
                interval = null;
            }
        });
        
    });
    
    
</script>
</html>