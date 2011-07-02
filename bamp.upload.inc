<?php

// $Id: bamp.upload.inc $

/**
 * @file
 * Code to create bamp upload forms
 */

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

  $form['form'] = array(
    '#markup' => '<A href="?q=bamp/Upload%20forms/Crawford%20Site%20CSV%20upload">Crawford Site CSV upload</A></br>
      <A href="?q=bamp/Upload%20forms/Crawford%20field%20results%20CSV%20upload">Crawford field results CSV upload</A></br>
      <A href="?q=bamp/Upload%20forms/Crawford%20lab%20results%20CSV%20upload">Crawford lab results CSV upload</A></br>
      <A href="?q=bamp/Upload%20forms/Marty%20field%20results%20CSV%20upload">Marty field results CSV upload</A></br>
      <A href="?q=bamp/Upload%20forms/Marty%20site%20results%20CSV%20upload">Marty site results CSV upload</A></br>',
  );

  return $form;
}

function bampupload_menu_callback() {
  return drupal_get_form('bampupload_form');
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

  $aa = csv2aa($csv_file_path, ';');

  $na = (count($aa) - 1);
  
//  $nu = '100';
  
  $nu = $na;

  drupal_set_message(t('There are ' . $na . ' entries in this file'));
  drupal_set_message(t($nu . ' of these entries were uploaded'));

  for ($j = 1; $j <= $nu; $j++) {

    $fid = $aa[$j][0];
    $group = 'crawford';
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

    if ($species == 'Xcoho') {
      $species = 'Coho';
    }
    
    $uid = user_uid_optional_to_arg('%');
    $username = user_name($uid);
    
    $sample = db_insert('bamp_field_results')
        ->fields(array(
          'group_name' => $group,
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
          'upload_user' => $username,
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

    $group = 'crawford';
    $name = $aa[$j][0];
    $craw_site_id = $aa[$j][1];
    $lat = $aa[$j][2];
    $lon = $aa[$j][3];
    $zone = $aa[$j][4];
    $area = $aa[$j][5];

     $uid = user_uid_optional_to_arg('%');
    $username = user_name($uid);
    
    $sample = db_insert('bamp_sites')
        ->fields(array(
          'group_name' => $group,
          'craw_site_id' => $craw_site_id,
          'name' => $name,
          'min_latitude' => $lat,
          'min_longitude' => $lon,
          'type' => '0',
          'description' => NULL,
          'farm_id' => NULL,
          'zone' => $zone,
          'area' => $area,
          'upload_user' => $username,
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

    $group = 'crawford';
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

    $sample = db_insert('bamp_lab_results')
        ->fields(array(
          'group_name' => $group,
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

//  $nu = '100';
  
  $nu = $na;
  
  drupal_set_message(t('There are ' . $na . ' entries in this file'));
  drupal_set_message(t($nu . ' of these entries were uploaded'));

  for ($j = 1; $j <= $nu; $j++) {

    $group = 'marty';
    $date = $aa[$j][0];
    $rep = $aa[$j][1];
    $route = $aa[$j][2];
    $site_num = $aa[$j][3];
    $species_num = $aa[$j][4];
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
    $route_let = substr($route, 0, 1);

    $fin = db_select('bamp_field_results', 'x')
        ->fields('x', array('sample_no'))
        ->condition('date', $date, '=')
        ->condition('rep_no', $rep, '=')
        ->condition('route', $route, '=')
        ->condition('site_no', $site_num, '=')
        ->orderBy('sample_no', 'DESC')
        ->execute()
        ->fetchAssoc();
    
    $fish_no = $fin['sample_no'];
    $fish_no++;
    $fid = 'K' . $date . '-' . $rep . '-' . $route_let . $site_num . '-' . $fish_no;
    
    $site_name = db_select('bamp_sites', 's')
        ->fields('s', array('name'))
        ->condition('year', $date, '=')
        ->condition('rep', $rep, '=')
        ->condition('route', $route, '=')
        ->condition('craw_site_id', $site_num, '=')
        ->execute()
        ->fetchAssoc();
    
    if ($species_num == '1') {
      $species = 'Pink';
    } 
    elseif ($species_num == '2') {
      $species = 'Chum';
    } 
    else {
      $species = NULL;
    }
    
    $uid = user_uid_optional_to_arg('%');
    $username = user_name($uid);
    
    $sample = db_insert('bamp_field_results')
        ->fields(array(
          'group_name' => $group,
          'fish_id' => $fid,
          'species' => $species,
          'site_name' => $site_name,
          'site_no' => $site_num,
          'route' => $route,
          'rep_no' => $rep,
          'sample_no' => $fish_no,
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
          'upload_user' => $username,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . $csv_file_name . ' submitted successfully'));
}

function bampupmartysite_form($form_state) {
  drupal_set_title(t('Marty site CSV upload form'));

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

function bampupmartysite_menu_callback() {
  return drupal_get_form('bampupmartysite_form');
}

function bampupmartysite_form_validate($form, &$form_state) {

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

function bampupmartysite_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = csv2aa($csv_file_path, ',');

  $na = (count($aa) - 1);

  drupal_set_message(t('There are ' . $na . ' entries in this file'));

  for ($j = 1; $j <= $na; $j++) {

    $group = 'marty';
    $year = $aa[$j][0];
    $date = $aa[$j][1];
    $rep = $aa[$j][2];
    $route = $aa[$j][3];
    $site_name = $aa[$j][4];
    $site_no = $aa[$j][5];
    $distance = $aa[$j][6];
    $lat = $aa[$j][7];
    $lon = $aa[$j][8];
//    These need to go into the environmental table
//    $temp = $aa[$j][9];
//    $sal = $aa[$j][10];

    if (!is_numeric($lat)) {
      $lat = 0;
    }
    
    if (! is_numeric($lon)) {
      $lon = 0;
    }
    
//    if (! is_numeric($temp)) {
//      $temp = NULL;
//    }
//    
//    if (!is_numeric($sal)) {
//      $sal = NULL;
//    }
    
     $uid = user_uid_optional_to_arg('%');
    $username = user_name($uid);
    
    $sample = db_insert('bamp_sites')
        ->fields(array(     
          'year' => $year,
          'date' => $date,
          'rep' => $rep,
          'route' => $route,
          'name' => $site_name,
          'craw_site_id' => $site_no,
          'distance' => $distance,
          'min_latitude' => $lat,
          'min_longitude' => $lon,
//          'temp' => $temp,
//          'sal' => $sal,
          'group_name' => $group,
          'upload_user' => $username,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . $csv_file_name . ' submitted successfully'));
}

/**
 * Create BAMP CSV upload form for Crawford data
 */
function bampupweather_form($form_state) {
  drupal_set_title(t('Weather CSV upload form'));

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

function bampupweather_menu_callback() {
  return drupal_get_form('bampupweather_form');
}

function bampupweather_form_validate($form, &$form_state) {

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

function bampupweather_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = csv2aa($csv_file_path, ' ');

  $na = (count($aa) - 1);
  
//  $nu = '1';
  
  $nu = $na;

  drupal_set_message(t('There are ' . $na . ' entries in this file'));
  drupal_set_message(t($nu . ' of these entries were uploaded'));
  
  for ($j = 1; $j <= $nu; $j++) {

//    Put the array through array_filter to strip out any blank elements
//    And then through array_values to rebuild the keys
   
    
//    $fa = array_values(array_filter($aa[$j]));
    
    $ia = implode(' ', $aa[$j]);
    
//    Stupid replacements due to the input data being really inconsistent
    $ia = str_replace("0-------", " 00.0N/A", $ia);
    $ia = str_replace(":30:00     ---     ---", ":30 00.0 00.0 00.0", $ia);
    $ia = str_replace(":00:00     ---     ---", ":00 00.0 00.0 00.0", $ia);
    $ia = str_replace(":30:00", ":30 00.0", $ia);
    $ia = str_replace(":00:00", ":00 00.0", $ia);
    $ia = str_replace("0---", " 00.0N/A", $ia);
    $ia = str_replace("---", " 00.0", $ia);
    $ia = str_replace("--", " 00.0", $ia);
    $ia = preg_replace('/\s+/', ' ', $ia);
    drupal_set_message($ia);
    $ea = explode(' ', $ia);
    
    $date = $ea[0];
    $time = $ea[1];
    $temp = $ea[2];
    $temp_hi = $ea[3];
    $temp_low = $ea[4];
    $hum = $ea[5];
    $dew_pt = (float) $ea[6];
    $wind_speed = $ea[7];
//    Wind_hi is given as a speed and direction with no separation
//    This code is to split the speed and direction out so that they can eb displayed separately
    $wind = $ea[8];
    $wind_hi = (float) $wind;
    $hi_count = strlen($wind_hi);
    $wind_dir = substr($wind, $hi_count);
    $rain = $ea[9];
    if (strlen($rain > 4)) {
      $rain = '0';
      $bar = $ea[9];
      $arc_int = $ea[10];
      $wind_dir_deg = $ea[11];
    }
    else {
      $rain = $ea[9];
      $bar = $ea[10];
      $arc_int = $ea[11];
      $wind_dir_deg = $ea[12];
    }
    
    $uid = user_uid_optional_to_arg('%');
    $username = user_name($uid);
   
//    drupal_set_message('Date: ' . $date); 
    drupal_set_message(t('Time: ' . $time)); 
    drupal_set_message(t('Temp: ' . $temp)); 
    drupal_set_message(t('Hi temp: ' . $temp_hi)); 
    drupal_set_message(t('Low temp: ' . $temp_low));
    drupal_set_message(t('Humidity: ' . $hum));
    drupal_set_message(t('Dew point: ' . $dew_pt));
    drupal_set_message(t('Wind speed: ' . $wind_speed));
    drupal_set_message(t('Wind high: ' . $wind_hi));
//    drupal_set_message('Wind dir: ' . $wind_dir);
    drupal_set_message(t('Rain: ' . $rain));
//    drupal_set_message('Bar: ' . $bar);
//    drupal_set_message('Arc int: ' . $arc_int);
//    drupal_set_message('Wind dir deg: ' . $wind_dir_deg);
    
    $sample = db_insert('bamp_weather')
        ->fields(array(
          'date' => $date,
          'time' => $time,
          'temp' => $temp,
          'temp_hi' => $temp_hi,
          'temp_low' => $temp_low,
          'hum' => $hum,
          'dew_pt' => $dew_pt,
          'wind_speed' => $wind_speed,
          'wind_hi' => $wind_hi,
          'wind_dir' => $wind_dir,
          'rain' => $rain,
          'bar' => $bar,
          'arc_int' => $arc_int,
          'wind_dir_deg' => $wind_dir_deg,
          'upload_user' => $username,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . $csv_file_name . ' submitted successfully'));
}

/**
 * Create BAMP CSV upload form for Crawford data
 */
function bampupenvironment_form($form_state) {
  drupal_set_title(t('Environment CSV upload form'));

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

function bampupenvironment_menu_callback() {
  return drupal_get_form('bampupenvironment_form');
}

function bampupenvironment_form_validate($form, &$form_state) {

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

function bampupenvironment_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = csv2aa($csv_file_path, ';');

  $na = (count($aa) - 1);
  
//  $nu = '100';
  
  $nu = $na;

  drupal_set_message(t('There are ' . $na . ' entries in this file'));
  drupal_set_message(t($nu . ' of these entries were uploaded'));

  for ($j = 1; $j <= $nu; $j++) {

    $fid = $aa[$j][0];
    $group = 'crawford';
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

    if ($species == 'Xcoho') {
      $species = 'Coho';
    }
    
    $uid = user_uid_optional_to_arg('%');
    $username = user_name($uid);
    
    $sample = db_insert('bamp_field_results')
        ->fields(array(
          'group_name' => $group,
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
          'upload_user' => $username,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . $csv_file_name . ' submitted successfully'));
}

