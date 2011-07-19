<?php

// $Id: bamp.map.inc $

/**
 * @file
 * Code to create bamp map displays
 */
include 'maps/googlemapapiv3.inc';
include 'maps/JSMin.php';

function bamp_maps_form($form_state) {
  drupal_set_title(t('Map menu'));

  $form = array();

  $form['form'] = array(
    '#markup' => '<p>This is the BAMP map menu</p>',
  );

  return $form;
}

function bamp_maps_menu_callback() {
  return drupal_get_form('bamp_maps_form');
}

function bamp_maps1_form($form_state) {
  drupal_set_title(t('Map page'));

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => 'Map',
  );

  drupal_add_js('http://maps.google.com/maps/api/js?sensor=false', FALSE, TRUE, FALSE);

  $map = new GoogleMapAPIv3();
  $map->_minify_js = isset($_REQUEST["min"]) ? FALSE : TRUE;
  $map->disableSidebar();
  
  $yellow_icon = "http://bamp.discoverygarden.ca/sites/bamp.discoverygarden.ca/files/fish-icon-red.png";
//  $yellow_icon_key = $map->setMarkerIconKey($yellow_icon);
  $blue_icon = "http://bamp.discoverygarden.ca/sites/bamp.discoverygarden.ca/files/fish-icon-blue.png";
  $blue_icon_key = $map->setMarkerIconKey($blue_icon);
  $green_icon = "http://bamp.discoverygarden.ca/sites/bamp.discoverygarden.ca/files/fish-icon-green.png";
  $green_icon_key = $map->setMarkerIconKey($green_icon);
      
  var_dump($blue_icon_key);
  var_dump($green_icon_key);
  
  
  $select = db_select('bamp_sites', 's');
  $select->addField('s', 'min_latitude');
  $select->addField('s', 'min_longitude');
  $select->addField('s', 'name');
  $select->addField('s', 'id');
  $select->addField('s', 'group_name');
  $result = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $nm = count($result);

//  $marker = $map->addMarkerByCoords('-125', '50', '', '', '', $blue_icon);
//  
//  $marker2 = $map->addMarkerByCoords('-126', '50', '', '', '', $green_icon);
  
  
  for ($i = 0; $i < 100; $i++) {

    if ($result[$i]['min_longitude'] != 0) {
      $name = $result[$i]['name'];
      $id = $result[$i]['id'];
      $latitude = $result[$i]['min_latitude'];
      $longitude = $result[$i]['min_longitude'];
      $group_name = $result[$i]['group_name'];
      
      if ($i < 50) {
      $marker1 = $map->addMarkerByCoords($longitude, $latitude, $name, '<a href="?q=bamp/Edit/Site/' . $id . '">' . $name . '</a></br>Latitude: ' . $latitude . '</br>Longitude: ' . $longitude, '', $blue_icon);
      }
      else {        
      $marker2 = $map->addMarkerByCoords($longitude, $latitude, $name, '<a href="?q=bamp/Edit/Site/' . $id . '">' . $name . '</a></br>Latitude: ' . $latitude . '</br>Longitude: ' . $longitude, '', $green_icon);        
      }       
    }
  }
  
  
  $map->disableZoomEncompass();
  $map->setBoundsFudge('0.001');
  $map->setZoomLevel('8');
  $map->printSidebar();
  $map->printOnLoad();
  
  drupal_add_js($map->getMapJS(), 'inline');
  
  $form['form']['map_div1'] = array(
    '#type' => 'markup',
    '#markup' => <<<EOL
  <script type="text/javascript">window.onload=onLoadmap;</script>
EOL
      ,
  );
  
  $form['form']['map_div2'] = array(
    '#type' => 'markup',
    '#markup' => <<<EOF
    <script type="text/javascript">document.write('<div id="map" style="width: 500px; height: 500px; position:relative;"><\/div>');</script>
EOF
      ,
  );

  $print_map = $map->getHeaderJS();

  $form['form']['map_div3'] = array(
    '#type' => 'markup',
    '#markup' => $print_map,
  );

  return $form;
}

function bamp_maps1_menu_callback() {
  return drupal_get_form('bamp_maps1_form');
}