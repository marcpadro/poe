<?php

include_once('config.php');

$message = '';
$through_proxy = isset($_SERVER['HTTP_X_P01MPF']);

if ( isset($_POST['submit_image']) && $through_proxy ) {
    
    $error = $_FILES['image']['error'];
    
    if ($error) {
	$message = empty($_FILES['image']['name']) ? 'No file selected' : 'Problem uploading the file';
	$class = 'problem';
    } else {
	list($ok, $message) = process_image($_FILES['image']['tmp_name']);
	$class = $ok ? 'correct' : 'problem';
    }
}

if ( isset($_POST['submit_text']) && $through_proxy) {
    
    $path = TXT_QUEUE_DIR . (int)(microtime(true)*1000000) . '.txt';
    if (file_put_contents($path, $_POST['text']) == false) {
	$message = 'Problem saving the text';
	$class = 'problem';
    }
    else {
	$message = 'Text added to queue';
	$class = 'correct';
    }
    
}


function process_image($path) {
    
    $image = get_image($path);
    
    if ( !$image ) return array(false, 'Invalid File');
    else {
	
	$new_image = imagecreatetruecolor(IMG_WIDTH, IMG_HEIGHT);
	
	$width = imagesx($image);
	$height = imagesy($image);
	
	$width_ratio = $width / IMG_WIDTH;
	$height_ratio = $height / IMG_HEIGHT;
	if ( $width_ratio < $height_ratio) {
	    $height_crop = $width_ratio * IMG_HEIGHT;
	    $height_offset = ($height - $height_crop) / 2; 
	    imagecopyresampled($new_image, $image, 0, 0, 0, $height_offset, IMG_WIDTH, IMG_HEIGHT, $width, $height_crop);
	} else {
	    $width_crop = $height_ratio * IMG_WIDTH;
	    $width_offset = ($width - $width_crop) / 2; 
	    imagecopyresampled($new_image, $image, 0, 0, $width_offset, 0, IMG_WIDTH, IMG_HEIGHT, $width_crop, $height);
	}
	
	$new_path = IMG_QUEUE_DIR . (int)(microtime(true)*1000000) . '.png';
       
	if ( !imagepng($new_image, $new_path) ) {
	    return  array(false, "Can't save image");
	}
	return  array(true, "Image added to queue");
    }
}

function get_image($path) {
    $info = getimagesize($path);
    if ($info && $info[0] && $info[1]) {
	    
	switch ($info[2]) {
	    case IMAGETYPE_GIF:
		return imagecreatefromgif($path);
	    
	    case IMAGETYPE_JPEG:
		return imagecreatefromjpeg($path);
	    
	    case IMAGETYPE_PNG:
		return imagecreatefrompng($path);
	}
    }
    
    return 0;
}
?>
<!DOCTYPE html>

<html>
<head>
    <meta charset="utf-8">
    <title>Proxy 0.0.0.1</title>
    <link type="text/css" rel="stylesheet" href="tfm.css" />
</head>

<body>
    
    <nav>
	<a href="#que">Qué</a>
	<a href="#donde">Dónde</a>
	<a href="#como">Cómo usar</a>
        <a href="#paraque">Para qué</a>
        <a href="#funcionamiento">Funcionamiento</a>
        <a href="#creditos">Créditos</a>
    </nav>
    
    <div id="fitxa">
    
	<h1><span class='marker'>Proxy 0.0.0.1</span></h1>
	<img id='image' src="getImage.php" width="<?php echo IMG_WIDTH; ?>px" height="<?php echo IMG_HEIGHT; ?>px" />
	<pre id="text"><?php include 'getText.php'; ?></pre>

	<?php if (!empty($message)): ?>
	<h3 id='missatge' class='<?php echo $class; ?>'><?php echo $message ?></h3>
	<?php endif; ?>
	
	<?php if ($through_proxy): ?>
	    <div id="template_forms">
		<div class='template_form_shower' id='image_form_shower'>
		    <input type='radio' name='templates' id='show_image_form' class='shower show_image_form'style='display:none;'/>
		    <label for='show_image_form' class='shower show_image_form'>+ Submit image template</label>
		    <input type='radio' name='templates' id='hide_image_form' class='hider hide_image_form'style='display:none;'/>
		    <label for='hide_image_form' class='hider hide_image_form'>- Submit image template</label>
		    <form method='post' enctype=multipart/form-data action='' id='image_form' class='template_form'>
			<input type='file' name='image' accept='image/gif, image/jpeg, image/png'/>
			<br/>
			<input type='submit' name='submit_image' value='submit image' />
		    </form>
		</div>
		
		<div class='template_form_shower' id='text_form_shower'>
		    <input type='radio' name='templates' id='show_text_form' class='shower show_text_form'style='display:none;'/>
		    <label for='show_text_form' class='shower show_text_form'>+ Submit text template</label>
		    <input type='radio' name='templates' id='hide_text_form' class='hider hide_text_form'style='display:none;'/>
		    <label for='hide_text_form' class='hider hide_text_form'>- Submit text template</label>
		    <form method='post' action='' id='text_form' class='template_form'>
			<textarea name='text' id='textarea' cols='60' rows='5' maxlength='256'></textarea>
			<br/>
			<input type='submit' name='submit_text' value='submit text' />
		    </form>
		</div>
	    </div>
	<?php endif; ?>
    
    </div>
    
    <hr/>
    
    <section id="que" class="section">
        <header>
            <h2 class="section-title">Qué</h2>
        </header>
	<p>
            Soy un ordenador conectad@ a la red, ejecutando un servidor proxy
            que permite navegar el Web usándome a mí como intermediari@, y
            también ejecuto un servidor Web que me permite alojar esta página.
            Intento añadir un poco de privacidad a tu navegación y al mismo
            tiempo uso (de forma no reconocible) los contenidos que circulan a
            través mio para configurar mi propia identidad. Puedes ser parte de
            ello si configuras tu navegador para usarme como Proxy. Entonces,
            además, podrás ayudarme a definir mi identidad escogiendo una imagen
            y un texto que yo usaré como referencia.
	</p>
    </section>
    <section id="donde" class="section">
        <header>
            <h2 class="section-title">Dónde</h2>
	</header>
        <p>
            Estoy situad@ en la sala polivalente del edificio Tànger del Campus
            Poblenou de la Universitat Pompeu Fabra, en la calle Tànger,
            122-140, 08018 Barcelona; aproximadamente en latitud 41,4031 y
            longitud 2,1942.
	</p>
        <p>
            Mi dirección IP es 193.145.46.8
        </p>
    </section>
    
    <section id="como" class="section">
        <header>
            <h2 class="section-title">Cómo usar</h2>
        </header>
	<p>
            Debes configurar tu navegador para que use el Proxy en la dirección
            193.145.46.8 y en el puerto 8118. Este ajuste se encuentra
            generalmente en la Configuración Avanzada, a veces dentro del
            apartado “Conexión” o “Red” y bajo el nombre “Proxy”, “Proxies” o
            “Servidor Intermediario”. Solo funciono sobre los protocolos HTTP y
            HTTPS.
	</p>
	<p>
            Una vez configurado puedes usar tu navegador con normalidad; en las
            conexiones no encriptadas (HTTP) podrás ver un recuadro que indica
            que estás navegando a través mio.
	</p>
        <p>
            Si vuelves a entrar en esta página una vez conectado al Proxy
            tendrás la opción de definir la imagen y el texto que uso como
            guías.
        </p>
        <p>
            Para obtener mejores resultados debes eliminar las cookies una vez
            hayas configurado el Proxy y antes de empezar a navegar.
        </p>
    </section>
    
    <section id="paraque" class="section">
        <header>
            <h2 class="section-title">Para qué</h2>
        </header>
	<p>
            Puedes usarme para mejorar un poco tu privacidad respecto a los
            servidores que contienen las páginas Web que consultas,
            especialmente para evitar que identifiquen tu localización
            geográfica. No puedo garantir que los resultados sean siempre
            satisfactorios, puesto que existen múltiples y complejos métodos
            para extraer información de los usuarios del Web, pero hago lo que
            puedo para evitarlo.
        </p>
        <p>
            Solo puedo ayudarte respecto a la información que reciben los
            servidores de las páginas que consultas. Si lo que te preocupa es
            que la información pueda ser interceptada en mitad del camino (por
            ataques de ciber-criminales o por el control de ciertas
            organizaciones), entonces no te puedo ayudar. Pero puedes probar de
            usarme en conjunción con <a href="https://www.torproject.org">Tor</a>.
        </p>
        <p>
            Principalmente puedo ser de utilidad para personas corrientes que
            quieran navegar de forma corriente pero sin que se aprovechen de su
            información para condicionar el contenido que reciben.
        </p>
        <p>
            Además también puedes usarme solo para ayudarme a construir mi
            identidad, puesto que dependo de tu participación para ello.
        </p>
        <p>
            No me hago responsable, de todas formas, del uso que puedas hacer de
            este servicio ni de las consecuencias que pueda comportar.
	</p>
    </section>
    <section id="funcionamiento" class="section">
        <header>
            <h2 class="section-title">Funcionamiento</h2>
        </header>
	<p>
            Cada vez que consultas una página web estas enviando una petición al
            servidor que la contiene. Con esta petición se envía cierta
            información que no es muy trascendental pero que puede ser
            suficiente para condicionar el contenido que recibes o para
            registrar los comportamientos de múltiples usuarios.
        </p>
        <p>
            Quizá el parámetro más significativo de esta información es la
            dirección IP, que define el punto desde el cual accedes a Internet y
            que, por lo tanto, puede servir para identificarte como usuario o
            para deducir tu localización geográfica. Otros parámetros de
            información que también se envían pueden ser: qué navegador y qué
            sistema operativo usas, qué idioma prefieres o la página que te ha
            enlazado a la página que quieres consultar.
        </p>
        <p>
            Al configurar tu navegador para usarme a mí, lo que estás haciendo
            es mandarme a mí la petición en vez de al servidor, y yo me encargo
            de enviar la petición en mi nombre, modificándola en la medida de lo
            posible para que no rebele información significativa sobre ti.
        </p>
        <p>
            Entonces el servidor me envía la respuesta con el contenido y yo te
            la reenvío a ti, modificando también algunas partes que puedan
            comprometer tu privacidad.
        </p>
        <p>
            Además, cada texto e imagen que reenvío a un usuario lo desordeno
            (por píxeles o caracteres) y lo almaceno para usarlo como “materia
            prima” en la generación de la imagen y el texto que podéis ver al
            principio de esta página, basándome en una imagen y un texto que los
            usuarios pueden definir.
	</p>
        <p>
            A veces, pero, la comunicación entre el navegador y el servidor es
            encriptada (HTTPS), con lo cual yo no puedo descifrarla ni
            modificarla (y eso es bueno porque si no deberías confiar muuucho en
            mí). En tales casos me permito algunas licencias creativas y
            modifico el texto y la imagen según me parece.
        </p>
        <p>
            A pesar de todo esto, en ningún caso registro información sobre la
            navegación de los usuarios.
        </p>
	<p>
	    Puedes consultar el código fuente del proyecto en <a href="https://github.com/marcpadro/poe">GitHub</a>.
	</p>
    </section>
    <section id="creditos" class="section">
        <header>
            <h2 class="section-title">Créditos</h2>
        </header>
        <p>
            El servidor proxy que estoy usando es una modificación personalizada
            de un software de código abierto llamado <a href="http://www.privoxy.org">Privoxy</a>.
        </p>
        <p>
            Mi funcionamiento está basado también en otros recursos gratuitos y
            de código abierto: <a href="https://www.debian.org">Debian</a>,
            <a href="http://httpd.apache.org">Apache HTTP Server</a>,
            <a href="http://php.net">PHP</a>,
            <a href="http://openframeworks.cc">openFrameworks</a> y
            <a href="https://github.com/jlblancoc/nanoflann">Nanoflann</a>.
        </p>
        <p>
            Este proyecto forma parte de un Trabajo Final del <a href="http://www.idec.upf.edu/master-universitario-en-artes-digitales">Máster
            Universitario en Artes Digitales</a> del IDEC-UPF.
        </p>
    </section>

</body>

<script type='text/javascript' src='http://code.jquery.com/jquery-1.10.2.min.js' ></script>
<script type='text/javascript'>
    $( document ).ready( function(){
	setInterval( function(){
	
		$('#image').attr('src', "getImage.php?"+Math.random());
		
		$('#text').load('getText.php');

	    }, 1000);
    });
    
    
</script>
</html>

<?php

?>
