<?php
/*
   Run this first with an older PHP interpreter, and then with a newer
   version. This way you get a list (once) of added functions (the func
   list is stored in a temporary file).
*/
$DATA_FILE = "/tmp/ckavail.lst";

#-- load previous list
if ($f = @fopen($DATA_FILE, "rb")) {
   $oldl = unserialize(fread($f, 1<<20));
   fclose($f);
}
//echo "OLD=";print_r($oldl);

#-- current func list
$newl = get_defined_stuff();
//echo "NEW=";print_r($newl);

#-- save this one
if ($f = fopen($DATA_FILE, "wb")) {
   fwrite($f, serialize($newl));
   fclose($f);
}
else {
   echo "ERROR(could not save current list)\n";
}

#-- comparison of  current / last run
echo "old(".count($oldl["functions"])."), new(".count($newl["functions"]).")\n";
if ($oldl) {

   #-- funcs
   $new = array_diff($newl["functions"], $oldl["functions"]);
   echo "New functions added between PHP $oldl[version] and current $newl[version] interpreter:\n";
   print_r($new);

   #-- constants
   $new = array_diff(array_keys($newl["constants"]), array_keys($oldl["constants"]));
   echo "\nNewly added constants:\n";
   print_r($new);

   #-- classes
   $new = array_diff($newl["classes"], $oldl["classes"]);
   echo "\nAnd classes:\n";
   print_r($new);

}
else {
   echo "Now start this script a 2nd time with the newer interpreter version.\n";
}


function get_defined_stuff() {
   $rl = array();
   $f = get_defined_functions();
   while ($f["internal"]) {
      $f = $f["internal"];
   }
   $rl["functions"] = $f;
   $c = get_defined_constants();
   $rl["constants"] = $c;
   $c = get_declared_classes();
   $rl["classes"] = $c;
   $rl["version"] = PHP_VERSION;
   return($rl);
}


?>