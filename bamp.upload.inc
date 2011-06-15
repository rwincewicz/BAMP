<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Create BAMP upload menu
 */
function bampupload_form($form_state) {
  drupal_set_title(t('Upload menu'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p> This is the BAMP upload menu</p>',
  );

  return $form;
}

function bampupload_menu_callback() {
  return drupal_get_form('bampupload_form');
}

/**
 * Create BAMP CSV upload form
 */
function bampupcsv_form($form_state) {
  drupal_set_title(t('CSV upload form'));

  $form = array();

  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSV upload'),
  );

  $form['form']['csv'] = array(
    '#type' => 'file',
    '#title' => t('Select a CSV file'),
    '#size' => 40,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#executes_submit_callback' => TRUE,
  );

  return $form;
}

function bampupcsv_menu_callback() {
  return drupal_get_form('bampupcsv_form');
}

function bampupcsv_form_validate($form, &$form_state) {

  $validators = array('file_validate_extensions' => array('csv'));
  $file = file_save_upload('csv', $validators);

  if (!$file)
    form_set_error('upload', 'You must select a valid file to upload.');
  else {
    // Manually add the uploaded file to the $form_state
    $form_state['values']['csv']['title'] = $file->filename;
    $form_state['values']['csv']['file'] = $file;
    $form_state['values']['csv']['filepath'] = $file->destination;
  }
}

function bampupcsv_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = csv2aa($csv_file_path, ',');
  
  // Variables common to all the sample batches in the file
  $site = $aa[0][3];
  $tfish = $aa[0][10];
  $method = $aa[9][2];


  drupal_set_message(t('Site: ' . $site));
  drupal_set_message(t('Number of fish: ' . $tfish));
  drupal_set_message(t('Method: ' . $method));


  $mid = db_query("SELECT id FROM {bamp_method} WHERE name = :method", array(':method' => $method))->fetchField();

  if (empty($mid)) {
    drupal_set_message(t('Invalid method in CSV'));
  }

  $sid = db_query("SELECT id FROM {bamp_sites} WHERE name = :site", array(':site' => $site))->fetchField();

  if (empty($sid)) {
    drupal_set_message(t('Invalid site in CSV'));
  }

  drupal_set_message(t('Site ID: ' . $sid));

  for ($i = 0; $i <= 2; $i++) {

    // Variables that may vary between sample batches

    if ($i == 0) {
      $ddiff = 0;
      $udiff = 0;
      $pdiff = 0;
    }
    elseif ($i == 1) {
      $ddiff = 47;
      $udiff = 46;
      $pdiff = 47;
    }
    else {
      $ddiff = 100;
      $udiff = 99;
      $pdiff = 100;
    }

    $date = $aa[7 + $ddiff][1];
    $user = $aa[7 + $udiff][5];
    $pen = $aa[8 + $pdiff][1];

    $mdate = ($date * 86400);

    $rdate = date('d-M-y H:m:s', $timestamp = $mdate);

    drupal_set_message(t('Date: ' . $rdate));
    drupal_set_message(t('User: ' . $user));
    drupal_set_message(t('Pen: ' . $pen));

    for ($j = 0; $j <= 19; $j++) {

      // Variables that very within a sample batch

      $rid = next_id('bamp_results');
      $nid = $rid;
      $fid = $aa[11 + $pdiff + $j][0];
      $size = $aa[11 + $pdiff + $j][1];
      if ($size == '') {
        $size = '0';
      }
      $chal = $aa[11 + $pdiff + $j][2];
      if ($chal == '') {
        $chal = '0';
      }
      $pre = $aa[11 + $pdiff + $j][3];
      if ($pre == '') {
        $pre = '0';
      }
      $male = $aa[11 + $pdiff + $j][4];
      if ($male == '') {
        $male = '0';
      }
      $female = $aa[11 + $pdiff + $j][5];
      if ($female == '') {
        $female = '0';
      }
      $grav = $aa[11 + $pdiff + $j][6];
      if ($grav == '') {
        $grav = '0';
      }
      $cal = $aa[11 + $pdiff + $j][7];
      if ($cal == '') {
        $cal = '0';
      }
      $comm = $aa[11 + $pdiff + $j][10];

      drupal_set_message(t('Result ID: ' . $rid));
      drupal_set_message(t('Fish ID: ' . $fid));
      drupal_set_message(t('Size: ' . $size));
      drupal_set_message(t('Chalimus: ' . $chal));
      drupal_set_message(t('Pre-adult: ' . $pre));
      drupal_set_message(t('Male: ' . $male));
      drupal_set_message(t('Female: ' . $female));
      drupal_set_message(t('Gravid: ' . $grav));
      drupal_set_message(t('Caligus: ' . $cal));
      drupal_set_message(t('Comments: ' . $comm));

      $sample = db_insert('bamp_results')
          ->fields(array(
            'id' => $rid,
            'fish_id' => $fid,
            'caligus' => $cal,
            'chalimus' => $chal,
            'female' => $female,
            'gravid' => $grav,
            'lab_id' => '0',
            'male' => $male,
            'method' => $mid,
            'user' => $user,
            'pre_adult' => $pre,
            'scarring' => '0',
            'comments' => $comm,
            'date' => $mdate,
            'site_id' => $sid,
            'pen' => $pen,
            'tot_fish' => $tfish,
            'size' => $size,
          ))
          ->execute();

      dummy_node($form, $form_state, $nid);
    }
  }

  drupal_set_message(t('File ' . $csv_file_name . ' submitted successfully'));

//  } catch (Exception $e) {
//    drupal_set_message(t('Invalid values were found in the CSV file'), 'error', FALSE);
//  }
}

/**
 * Create BAMP CSV upload form for Crawford data
 */
function bampupcrawfield_form($form_state) {
  drupal_set_title(t('Crawford field CSV upload form'));

  $form = array();

  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field CSV upload'),
  );

  $form['form']['csv'] = array(
    '#type' => 'file',
    '#title' => t('Select a CSV file'),
    '#size' => 40,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#executes_submit_callback' => TRUE,
  );

  return $form;
}

function bampupcrawfield_menu_callback() {
  return drupal_get_form('bampupcrawfield_form');
}

function bampupcrawfield_form_validate($form, &$form_state) {

  $validators = array('file_validate_extensions' => array('csv'));
  $file = file_save_upload('csv', $validators);

  if (!$file)
    form_set_error('upload', 'You must select a valid file to upload.');
  else {
    // Manually add the uploaded file to the $form_state
    $form_state['values']['csv']['title'] = $file->filename;
    $form_state['values']['csv']['file'] = $file;
    $form_state['values']['csv']['filepath'] = $file->destination;
  }
}

function bampupcrawfield_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = csv2aa($csv_file_path, ',');

  $na = (count($aa) - 1);

  drupal_set_message(t('There are ' . $na . ' entries in this file'));

  for ($j = 1; $j <= $na; $j++) {

    $rid = next_id('bamp_field_results');
    $nid = $rid;
    $site = $aa[$j][7];
    $site_name = trim(str_replace("  ", " ", $site));
    $method = '0';
    $date = $aa[$j][3];
    $crew = $aa[$j][9];
    $fid = $aa[$j][0];
    $trip_id = $aa[$j][1];
    $species = $aa[$j][2];
    $set_no = $aa[$j][4];
    $blind_no = $aa[$j][5];
    $lab_id = $aa[$j][6];
    $samp_id = $aa[$j][8];
    $comm = $aa[$j][25];
    $tlice = $aa[$j][12];

    drupal_set_message(t('Result ID: ' . $rid));
    drupal_set_message(t('Site name: ' . $site_name));
    drupal_set_message(t('Date: ' . $date));
    drupal_set_message(t('Sample number: ' . $samp_id));
    drupal_set_message(t('Method: ' . $method));
    drupal_set_message(t('Crew: ' . $crew));
    drupal_set_message(t('Fish ID: ' . $fid));
    drupal_set_message(t('Trip ID: ' . $trip_id));
    drupal_set_message(t('Species: ' . $species));
    drupal_set_message(t('Set no: ' . $set_no));
    drupal_set_message(t('Blind no: ' . $blind_no));
    drupal_set_message(t('Lab: ' . $lab_id));
    drupal_set_message(t('Comments: ' . $comm));
    drupal_set_message(t('Total lice: ' . $tlice));

    $sample = db_insert('bamp_field_results')
        ->fields(array(
          'id' => $rid,
          'fish_id' => $fid,
          'sample_id' => $samp_id,
          'trip_id' => $trip_id,
          'species' => $species,
          'set_no' => $set_no,
          'blind_no' => $blind_no,
          'lab' => $lab_id,
          'site_name' => $site_name,
          'method' => $method,
          'crew' => $crew,
          'comments' => $comm,
          'date' => $date,
          'total_lice' => $tlice,
          
//              'scarring' => '0',
//              'pre_adult' => $pre,          
//              'caligus' => $cal,
//              'chalimus' => $chal,
//              'pre_adult' => $pre,
//              'female' => $female,
//              'gravid' => $grav,              
//              'male' => $male,
//              'pen' => $pen,
//              'tot_fish' => $tfish,
//              'size' => $size,
        ))
        ->execute();

//    if (check_node($nid) == FALSE) {
//      dummy_node($form, $form_state, $nid);
//      drupal_set_message(t('Dummy node doesn\'t exist, creating'));
//    }
//    else {
//      drupal_set_message(t('Dummy node already exists'));
//    }
  }

  drupal_set_message(t('File ' . $csv_file_name . ' submitted successfully'));
}

/**
 *
 * @param type $form_state
 * @return string 
 * 
 * Create an upload form for Sites from Crawford's team
 */
function bampupcrawsite_form($form_state) {
  drupal_set_title(t('Crawford Site CSV upload form'));

  $form = array();

  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSV upload'),
  );

  $form['form']['csv'] = array(
    '#type' => 'file',
    '#title' => t('Select a CSV file'),
    '#size' => 40,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#executes_submit_callback' => TRUE,
  );

  return $form;
}

function bampupcrawsite_menu_callback() {
  return drupal_get_form('bampupcrawsite_form');
}

function bampupcrawsite_form_validate($form, &$form_state) {

  $validators = array('file_validate_extensions' => array('csv'));
  $file = file_save_upload('csv', $validators);

  if (!$file)
    form_set_error('upload', 'You must select a valid file to upload.');
  else {
    // Manually add the uploaded file to the $form_state
    $form_state['values']['csv']['title'] = $file->filename;
    $form_state['values']['csv']['file'] = $file;
    $form_state['values']['csv']['filepath'] = $file->destination;
  }
}

function bampupcrawsite_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = csv2aa($csv_file_path, ',');
  $na = (count($aa) - 1);

  drupal_set_message(t('There are ' . $na . ' sites in this file'));
  for ($j = 1; $j <= $na; $j++) {

    $sid = next_id('bamp_sites');
    $name = $aa[$j][0];
    $craw_site_id = $aa[$j][1];
    $lat = $aa[$j][2];
    $lon = $aa[$j][3];
    $zone = $aa[$j][4];
    $area = $aa[$j][5];

//        drupal_set_message(t('Site: ' . $name));
//        drupal_set_message(t('Site ID: ' . $sid));
//        drupal_set_message(t('Site ID Crawford: ' . $craw_site_id));
//        drupal_set_message(t('Latitude: ' . $lat));
//        drupal_set_message(t('Longitude: ' . $lon));
//        drupal_set_message(t('Zone: ' . $zone));
//        drupal_set_message(t('Area: ' . $area));

    $sample = db_insert('bamp_sites')
        ->fields(array(
          'id' => $sid,
          'craw_site_id' => $craw_site_id,
          'name' => $name,
          'max_latitude' => NULL,
          'max_longitude' => NULL,
          'min_latitude' => $lat,
          'min_longitude' => $lon,
          'type' => '0',
          'description' => NULL,
          'farm_id' => NULL,
          'zone' => $zone,
          'area' => $area,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . $csv_file_name . ' submitted successfully'));
}

/**
 *
 * @param type $form_state
 * @return string 
 * 
 * Create an upload form for Lab results from Crawford's team
 */
function bampupcrawlabr_form($form_state) {
  drupal_set_title(t('Crawford lab report CSV upload form'));

  $form = array();

  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSV upload'),
  );

  $form['form']['csv'] = array(
    '#type' => 'file',
    '#title' => t('Select a CSV file'),
    '#size' => 40,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#executes_submit_callback' => TRUE,
  );

  return $form;
}

function bampupcrawlabr_menu_callback() {
  return drupal_get_form('bampupcrawlabr_form');
}

function bampupcrawlabr_form_validate($form, &$form_state) {

  $validators = array('file_validate_extensions' => array('csv'));
  $file = file_save_upload('csv', $validators);

  if (!$file)
    form_set_error('upload', 'You must select a valid file to upload.');
  else {
    // Manually add the uploaded file to the $form_state
    $form_state['values']['csv']['title'] = $file->filename;
    $form_state['values']['csv']['file'] = $file;
    $form_state['values']['csv']['filepath'] = $file->destination;
  }
}

function bampupcrawlabr_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = csv2aa($csv_file_path, ',');
  $na = (count($aa) - 1);

  drupal_set_message(t('There are ' . $na . ' entries in this file'));
  
  for ($j = 1; $j <= $na; $j++) {

    $lrid = next_id('bamp_lab_results');
    $fid = $aa[$j][0];
    $trip_id = $aa[$j][1];
    $group_id = $aa[$j][2];
    $lab = $aa[$j][3];
    $date = $aa[$j][4];
    $fish_no = $aa[$j][6];
    $lspecies = $aa[$j][7];
    $length = $aa[$j][8];
    $weight = $aa[$j][9];
    $total_lice = $aa[$j][30];
    $comm = $aa[$j][32];
    
    if ($lspecies == '') {
      $lspecies = 'Same';
    }

    drupal_set_message(t('Lab Result ID: ' . $lrid));
    drupal_set_message(t('Fish ID: ' . $fid));
    drupal_set_message(t('Trip ID: ' . $trip_id));
    drupal_set_message(t('Group ID: ' . $group_id));
    drupal_set_message(t('Lab: ' . $lab));
    drupal_set_message(t('Date: ' . $date));
    drupal_set_message(t('Fish number: ' . $fish_no));
    drupal_set_message(t('Lab Species: ' . $lspecies));
    drupal_set_message(t('Length: ' . $length));
    drupal_set_message(t('Weight: ' . $weight));
    drupal_set_message(t('Total lice: ' . $total_lice));
    drupal_set_message(t('Comments: ' . $comm));

    $sample = db_insert('bamp_lab_results')
        ->fields(array(
          'id' => $lrid,
          'fish_id' => $fid,
          'trip_id' => $trip_id,
          'group_id' => $group_id,
          'lab' => $lab,
          'date' => $date,
          'species' => $lspecies,
          'length' => $length,
          'weight' => $weight,
          'total_lice' => $total_lice,
          'comments' => $comm,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . $csv_file_name . ' submitted successfully'));
  
}
