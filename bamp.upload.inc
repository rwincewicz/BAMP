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
    
    
    $fid = $aa[$j][0];
    $trip_id = $aa[$j][1];
    $species = $aa[$j][2];
    $date = $aa[$j][3];
    $set_no = $aa[$j][4];
    $blind_no = $aa[$j][5];
    $lab_id = $aa[$j][6];
    $site = $aa[$j][7];
    
    $site_name = trim(str_replace("  ", " ", $site));
    
    $samp_id = $aa[$j][8];
    $crew = $aa[$j][9];
    $scar_chal = $aa[$j][10];
    $scar_mob = $aa[$j][11];
    $tlice = $aa[$j][12];
    $total_cop = $aa[$j][13];
    $total_chal = $aa[$j][14];
    $lep_pre_male = $aa[$j][15];
    $lep_pre_female = $aa[$j][16];
    $total_lep_pre = $lep_pre_male + $lep_pre_female;
    $lep_adult_male = $aa[$j][17];
    $lep_adult_female = $aa[$j][18];
    $total_lep_adult = $lep_adult_male + $lep_adult_female;
    $total_lep = $lep_pre_male + $lep_pre_female + $lep_adult_male + $lep_adult_female;
    $cal_pre_male = $aa[$j][20];
    $cal_pre_female = $aa[$j][21];
    $total_cal_pre = $cal_pre_male + $cal_pre_female;
    $cal_adult_male = $aa[$j][22];
    $cal_adult_female = $aa[$j][23];
    $total_cal_adult = $cal_adult_male + $cal_adult_female;
    $total_cal = $cal_pre_male + $cal_pre_female + $cal_adult_male + $cal_adult_female;
    $comm = $aa[$j][25];
    $total_motile = $total_cal_pre + $total_cal_adult + $total_lep_pre + $total_lep_adult;

    $sample = db_insert('bamp_field_results')
        ->fields(array(
          'id' => $rid,
          'fish_id' => $fid,
          'sample_id' => $samp_id,
          'trip_id' => $trip_id,
          'species' => $species,
          'set_no' => $set_no,
          'blind_no' => $blind_no,
          'to_lab' => $lab_id,
          'site_name' => $site_name,
          'crew' => $crew,
          'comments' => $comm,
          'date' => $date,
          'total_lice' => $tlice,
          'scar_chal' => $scar_chal,
          'scar_mob' => $scar_mob,
          'total_cop' => $total_cop,
          'total_chal' => $total_chal,
          'lep_pre_male' => $lep_pre_male,
          'lep_pre_female' => $lep_pre_female,
          'total_lep_pre' => $total_lep_pre,
          'lep_adult_male' => $lep_adult_male,
          'lep_adult_female' => $lep_adult_female,
          'total_lep_adult' => $total_lep_adult,
          'total_lep' => $total_lep,
          'cal_pre_male' => $cal_pre_male,
          'cal_pre_female' => $cal_pre_female,
          'total_cal_pre' => $total_cal_pre,
          'cal_adult_male' => $cal_adult_male,
          'cal_adult_female' => $cal_adult_female,
          'total_cal_adult' => $total_cal_adult,
          'total_cal' => $total_cal,
          'total_motile' => $total_motile,
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

/**
 * Create BAMP CSV upload form for Marty data
 */
function bampupmartyfield_form($form_state) {
  drupal_set_title(t('Marty field CSV upload form'));

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

function bampupmartyfield_menu_callback() {
  return drupal_get_form('bampupmartyfield_form');
}

function bampupmartyfield_form_validate($form, &$form_state) {

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

function bampupmartyfield_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = csv2aa($csv_file_path, ',');

  $na = (count($aa) - 1);

  drupal_set_message(t('There are ' . $na . ' entries in this file'));

  for ($j = 1; $j <= $na; $j++) {

    $rid = next_id('bamp_field_results');
    
    $date = $aa[$j][0];
    $rep = $aa[$j][1];
    $route = $aa[$j][2];
    $site_num = $aa[$j][3];
    $species = $aa[$j][4];
    $length = $aa[$j][5];
    $height = $aa[$j][6];
    $lep_cop = $aa[$j][7];
    $cal_cop = $aa[$j][8];
    $total_cop = $lep_cop + $cal_cop;
    $chal_a = $aa[$j][9];
    $chal_b = $aa[$j][10];
    $total_chal = $chal_a + $chal_b;
    $mot_unid = $aa[$j][11];
    $lep_pre_male = $aa[$j][12];
    $lep_pre_female = $aa[$j][13];
    $lep_pre_unid = $aa[$j][14];
    $total_lep_pre = $lep_pre_male + $lep_pre_female + $lep_pre_unid;
    $lep_adult_male = $aa[$j][15];
    $lep_adult_female = $aa[$j][16];
    $lep_adult_unid = $aa[$j][17];
    $lep_gravid = $aa[$j][18];
    $total_lep_adult = $lep_adult_male + $lep_adult_female + $lep_adult_unid + $lep_gravid;
    $total_lep = $total_lep_pre + $total_lep_adult;
    $caligus = $aa[$j][19];
    $caligus_gravid = $aa[$j][20];
    $total_cal = $caligus + $caligus_gravid;
    $chal_scar = $aa[$j][21];
    $mot_scar = $aa[$j][22];
    $pred_marks = $aa[$j][23];
    $hem = $aa[$j][24];
    $mate_guard = $aa[$j][25];
    $pin_belly = $aa[$j][26];
    $tissue_damage = $aa[$j][27];
    $comm = $aa[$j][28];
    $total_motile = $total_lep + $total_cal;
    $total_lice = $total_cop + $total_chal + $total_cal + $total_lep;

    
    $sample = db_insert('bamp_field_results')
        ->fields(array(
          'id' => $rid,
//          'fish_id' => $fid,
         
        
          'species' => $species,
         
          'site_no' => $site_num,
          'route' => $route,
          'rep_no' => $rep,
          'length' => $length,
          'height' => $height,
          'comments' => $comm,
          'date' => $date,
          'total_lice' => $total_lice,
          'cop_lep' => $lep_cop,
          'cop_cal' => $cal_cop,
          'total_cop' => $total_cop,
          'chal_a' => $chal_a,
          'chal_b' => $chal_b,
          'total_chal' => $total_chal,
          'mot_unid' => $mot_unid,
          'lep_pre_male' => $lep_pre_male,
          'lep_pre_female' => $lep_pre_female,
          'lep_pre_unid' => $lep_pre_unid,
          'total_lep_pre' => $total_lep_pre,
          'lep_adult_male' => $lep_adult_male,
          'lep_adult_female' => $lep_adult_female,
          'lep_adult_unid' => $lep_adult_unid,
          'lep_gravid' => $lep_gravid,
          'total_lep_adult' => $total_lep_adult,
          'total_lep' => $total_lep,
          'caligus' => $caligus,
          'cal_gravid' => $caligus_gravid,
          'total_cal' => $total_cal,
          'scar_mob' => $mot_scar,
          'scar_chal' => $chal_scar,
          'pred_mark' => $pred_marks,
          'hem' => $hem,
          'mate_guard' => $mate_guard,
          'pin_belly' => $pin_belly,
          'tissue_damage' => $tissue_damage,
          'total_motile' => $total_motile,
        ))
        ->execute();

  }

  drupal_set_message(t('File ' . $csv_file_name . ' submitted successfully'));
}