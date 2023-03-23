<?php
    require_once '../../../users/init.php';
    require_once $abs_us_root.$us_url_root.'users/includes/template/prep.php';
    if (!securePage($_SERVER['PHP_SELF'])){die();}
?>

<?php
echo "This is ESP Based Rode Counter";
?>
<a href="/app/">App</a>