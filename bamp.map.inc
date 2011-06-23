<?php

function bamp_maps_form($form_state) {
  drupal_set_title(t('Map menu'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p>This is the BAMP map menu</p>',
  );

  return $form;
}

function bamp_maps_menu_callback() {
  return drupal_get_form('bamp_maps_form');
}

function bamp_maps1_form($form_state) {
  drupal_set_title(t('Map page'));

  $form = array();

  $form['form'] = array(
    '#markup' => '<p>This is the BAMP map</p>',
  );

  drupal_add_js('http://maps.google.com/maps/api/js?sensor=false');
  $map_data = '<script type="text/javascript">
  function initialize() {
    var latlng = new google.maps.LatLng(-34.397, 150.644);
    var myOptions = {
      zoom: 8,
      center: latlng,
      mapTypeId: google.maps.MapTypeId.ROADMAP
    };
    var map = new google.maps.Map(document.getElementById("map_canvas"),
        myOptions);
  }

</script>';
  
//  Create the div for the map to be placed in
  $form['chart'] = array(
    '#markup' => $map_data . '<div id="map_canvas"></div>',
  );
  
  return $form;
}

function bamp_maps1_menu_callback() {
  return drupal_get_form('bamp_maps1_form');
}
