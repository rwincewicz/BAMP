<?php

// $Id: bamp.map.inc $

/**
 * @file
 * Code to create bamp map displays
 */
include 'googlemapapiv3.inc';
include 'JSMin.php';

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
  
//  $map->setCenterCoords('-126.63505417', '50.60522917');
  
  $select = db_select('bamp_sites', 's');
      $select->addField('s', 'min_latitude');
      $select->addField('s', 'min_longitude');
      $select->addField('s', 'name');
      $select->addField('s', 'id');
  $result = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  $nm = count($result);
  
  for ($i = 0; $i < $nm; $i++) {
    
    if ($result[$i]['min_longitude'] != 0) {
      $name = $result[$i]['name'];
      $id = $result[$i]['id'];

  $map->addMarkerByCoords($result[$i]['min_longitude'], $result[$i]['min_latitude'], $name, '<a href="?q=bamp/Edit/Site/' . $id . '">' . $name . '</a>');
    }
  }
  
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