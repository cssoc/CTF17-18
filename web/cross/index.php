<?php
	session_start();
	$db = new SQLite3('db.db');

?>

<!doctype html>
<html lang="en">
	<head>
		<title>Admins Personal Webpage</title>
		<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
	<style>textarea{ width: 70%; height: 5cm;}</style>

	</head>
	<body>
	<div class="container">
		<div class="jumbotron">
			<h1>Admins Webpage</h1>
			<p>Hi! My name's Admin. You might know me as the guy who writes all these shitty exploitable services.
			Anyhow my webpage is work in progress, but I left you a message form and I promise to read all of your messages
			(but it might take me a couple of seconds).<p>
		</div>

	<?php if(isset($_GET['id']) && isset($_SESSION['id'])):
		$stmt = $db->prepare('SELECT author, msg FROM reports WHERE rowid=:id');
		$stmt->bindParam(':id', $_GET['id'], SQLITE3_INTEGER);
		$res = $stmt->execute();
		
		$data = False;
		if($res)
			$data = $res->fetchArray();
		if($data && $_SESSION['admin'] != 1 && $data['author'] != $_SESSION['id'])
			$data = False;

		if($data):
	?>
		<h2>Your message:</h2>
		<p><?php print($data['msg']) ?></p>

		<?php else: ?>

		<div class="alert alert-danger">
		Message could not be displayed.
		</div>

		<?php endif; ?>


	<?php elseif(isset($_SESSION['id'])):
		if($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['msg'])):
			$stmt = $db->prepare('INSERT INTO reports(author, msg) VALUES (:author, :msg)');
			$stmt->bindParam(':author', $_SESSION['id'], SQLITE3_INTEGER);
			$stmt->bindParam(':msg', $_POST['msg'], SQLITE3_TEXT);

			$res = $stmt->execute();

			if($res): ?>
			<div class="alert alert-success">
			Sent ;)
			</div>
			<?php else: ?>
			<div class="alert alert-danger">
			Message not sent; Something went wrong :(
			</div>
			<?php endif;
		else:
			?>
			<form class="form-signin" action="/index.php" method="POST">
			<h2 class="form-signin-heading">Enter a new message</h2>
			<textarea name="msg">Enter message here...</textarea>
			<br />
			<input type="submit" value="Send">
			</form>
			<?php
			$stmt = $db->prepare('SELECT rowid FROM reports WHERE author=:id');
			$stmt->bindParam(':id', $_SESSION['id'], SQLITE3_INTEGER);

			$res = $stmt->execute();
			print("<p>These are your previously submitted messages:</p>");
			while($data = $res->fetchArray()){
				print("<a href=\"/index.php?id={$data['rowid']}\">Message #{$data['rowid']}</a> <br />");
			}
		endif;

	?>

	<?php else: ?>
		<div class="alert alert-danger">
		Please log in or register to send messages.
		</div>

	<?php endif; ?>

	<footer class="page-footer font-small blue pt-4 mt-4">
		<div class="container-fluid text-center text-md-left">
		<?php if(isset($_SESSION['id'])): ?>
		<div class="row">
		Logged in as: <?php print($_SESSION['name']); ?>
		</div>
		<div class="row">
		<a href="logout.php">Logout</a>
		</div>
		<?php else: ?>
		<div class="row">
		<a href="login.php">Login</a>
		</div>
		<div class="row">
		<a href="register.php">Register</a>
		</div>
		<?php endif; ?>
		<div class="row">
		<a href="/">Index</a>
		</div>
		</div>
	</footer>

	</div>
	</body>
</html>
