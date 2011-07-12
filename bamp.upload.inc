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
function bamp_upload_form($form_state) {
  drupal_set_title(t('Upload menu'));

  $form = array();

  $form['form'] = array(
    '#markup' => '<p>This is the BAMP upload menu</p>',
  );
  
//  $form['form'] = array(
//    '#markup' => '<A href="?q=bamp/Upload%20forms/Crawford%20Site%20CSV%20upload">Crawford Site CSV upload</A></br>
//      <A href="?q=bamp/Upload%20forms/Crawford%20field%20results%20CSV%20upload">Crawford field results CSV upload</A></br>
//      <A href="?q=bamp/Upload%20forms/Crawford%20lab%20results%20CSV%20upload">Crawford lab results CSV upload</A></br>
//      <A href="?q=bamp/Upload%20forms/Marty%20field%20results%20CSV%20upload">Marty field results CSV upload</A></br>
//      <A href="?q=bamp/Upload%20forms/Marty%20site%20results%20CSV%20upload">Marty site results CSV upload</A></br>',
//  );

  return $form;
}

function bamp_upload_menu_callback() {
  return drupal_get_form('bamp_upload_form');
}

/**
 * Create BAMP CSV upload form for Crawford data
 */
function bamp_upload_crawford_field_form($form_state) {
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

function bamp_upload_crawford_field_menu_callback() {
  return drupal_get_form('bamp_upload_crawford_field_form');
}

function bamp_upload_crawford_field_form_validate($form, &$form_state) {

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

function bamp_upload_crawford_field_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ';');

  $na = (count($aa) - 1);

//  $nu = '100';

  $nu = $na;

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));
  drupal_set_message(t("@nu" . ' of these entries were uploaded', array('@na' => $nu)));

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
    $site_name = str_replace(',', ';', $site_name);

    $samp_id = $aa[$j][8];
    $crew = $aa[$j][9];
    $crew = str_replace(',', ';', $crew);
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
    $comm = str_replace(',', ';', $comm);
    $total_motile = $total_cal_pre + $total_cal_adult + $total_lep_pre + $total_lep_adult;

    if ($species == 'Xcoho') {
      $species = 'Coho';
    }

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

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

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

/**
 *
 * @param type $form_state
 * @return string 
 * 
 * Create an upload form for Sites from Crawford's team
 */
function bamp_upload_crawford_site_form($form_state) {
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

function bamp_upload_crawford_site_menu_callback() {
  return drupal_get_form('bamp_upload_crawford_site_form');
}

function bamp_upload_crawford_site_form_validate($form, &$form_state) {

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

function bamp_upload_crawford_site_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ',');
  $na = (count($aa) - 1);

  drupal_set_message(t('There are ' . "@na" . ' sites in this file', array('@na' => $na)));
  for ($j = 1; $j <= $na; $j++) {

    $group = 'crawford';
    $name = $aa[$j][0];
    $craw_site_id = $aa[$j][1];
    $lat = $aa[$j][2];
    $lon = $aa[$j][3];
    $zone = $aa[$j][4];
    $area = $aa[$j][5];

    $uid = user_id_optional_to_arg('%');
    $username = bamp_user_name($uid);

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

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

/**
 *
 * @param type $form_state
 * @return string 
 * 
 * Create an upload form for Lab results from Crawford's team
 */
function bamp_upload_crawford_lab_form($form_state) {
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

function bamp_upload_crawford_lab_menu_callback() {
  return drupal_get_form('bamp_upload_crawford_lab_form');
}

function bamp_upload_crawford_lab_form_validate($form, &$form_state) {

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

function bamp_upload_crawford_lab_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ';');
  $na = (count($aa) - 1);

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));

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
    $lep_c0 = $aa[$j][10];
    $lep_c1 = $aa[$j][11];
    $lep_c2 = $aa[$j][12];
    $lep_c3 = $aa[$j][13];
    $lep_c4 = $aa[$j][14];
    $lep_pam = $aa[$j][15];
    $lep_paf = $aa[$j][16];
    $lep_am = $aa[$j][17];
    $lep_af = $aa[$j][18];
    
    if ($aa[$j][19] !== '') {
      $total_lep = $aa[$j][19];
    }
    else {
      $total_lep = NULL;
    }
    
    $cal_c0 = $aa[$j][20];
    $cal_c1 = $aa[$j][21];
    $cal_c2 = $aa[$j][22];
    $cal_c3 = $aa[$j][23];
    $cal_c4 = $aa[$j][24];
    $cal_pam = $aa[$j][25];
    $cal_paf = $aa[$j][26];
    $cal_am = $aa[$j][27];
    $cal_af = $aa[$j][28];
    
    if ($aa[$j][29] !== '') {
      $total_cal = $aa[$j][29];
    }
    else {
      $total_cal = NULL;
    }

    if ($aa[$j][30] !== '') {
      $total_lice = $aa[$j][30];
    }
    else {
      $total_lice = NULL;
    }
    
    $lesions = $aa[$j][31];
    $lesions = str_replace(',', ';', $lesions);

    $comments = $aa[$j][32];
    $comments = str_replace(',', ';', $comments);
    
    $change_log = $aa[$j][33];
    $change_log = str_replace(',', ';', $change_log);
    
    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);
    
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
          'upload_user' => $username,
          'total_lep' => $total_lep,
          'total_caligus' => $total_cal,
          'total_lice' => $total_lice,
          'lep_c0' => $lep_c0,
          'lep_c1' => $lep_c1,
          'lep_c2' => $lep_c2,
          'lep_c3' => $lep_c3,
          'total_lep_c4' => $lep_c4,
          'lep_pam' => $lep_pam,
          'lep_paf' => $lep_paf,
          'lep_am' => $lep_am,
          'lep_af' => $lep_af,
          'cal_c0' => $cal_c0,
          'cal_c1' => $cal_c1,
          'cal_c2' => $cal_c2,
          'cal_c3' => $cal_c3,
          'total_cal_c4' => $cal_c4,
          'cal_pam' => $cal_pam,
          'cal_paf' => $cal_paf,
          'cal_am' => $cal_am,
          'cal_af' => $cal_af,
          'comments' => $comments,
          'group_name' => 'crawford',
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

/**
 * Create BAMP CSV upload form for Marty data
 */
function bamp_upload_marty_field_form($form_state) {
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

function bamp_upload_marty_field_menu_callback() {
  return drupal_get_form('bamp_upload_marty_field_form');
}

function bamp_upload_marty_field_form_validate($form, &$form_state) {

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

function bamp_upload_marty_field_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ',');

  $na = (count($aa) - 1);

//  $nu = '100';

  $nu = $na;

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));
  drupal_set_message(t("@nu" . ' of these entries were uploaded', array('@nu' => $nu)));

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
    $comm = str_replace(',', ';', $comm);
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
    $username = bamp_user_name($uid);

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

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

function bamp_upload_marty_site_form($form_state) {
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

function bamp_upload_marty_site_menu_callback() {
  return drupal_get_form('bamp_upload_marty_site_form');
}

function bamp_upload_marty_site_form_validate($form, &$form_state) {

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

function bamp_upload_marty_site_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ';');

  $na = (count($aa) - 1);

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));
  
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

    if (!is_numeric($lon)) {
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
    $username = bamp_user_name($uid);

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

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

/**
 * Create BAMP CSV upload form for Crawford data
 */
function bamp_upload_weather_form($form_state) {
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

function bamp_upload_weather_menu_callback() {
  return drupal_get_form('bamp_upload_weather_form');
}

function bamp_upload_weather_form_validate($form, &$form_state) {

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

function bamp_upload_weather_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $site = explode('_', $csv_file_name);

  $aa = bamp_csv2aa($csv_file_path, ' ');

  $na = (count($aa) - 1);

//  $nu = '1';

  $nu = $na;

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));
  drupal_set_message(t("@nu" . ' of these entries were uploaded', array('@nu' => $nu)));

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
    drupal_set_message(t($ia, array('@ia' => $ia)));
    $ea = explode(' ', $ia);

    $site_name = $site[0];

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
    $username = bamp_user_name($uid);

//    drupal_set_message('Date: ' . $date); 
//    drupal_set_message(t('Time: ' . $time));
//    drupal_set_message(t('Temp: ' . $temp));
//    drupal_set_message(t('Site ID: ' . $site_id));
//    drupal_set_message(t('Hi temp: ' . $temp_hi));
//    drupal_set_message(t('Low temp: ' . $temp_low));
//    drupal_set_message(t('Humidity: ' . $hum));
//    drupal_set_message(t('Dew point: ' . $dew_pt));
//    drupal_set_message(t('Wind speed: ' . $wind_speed));
//    drupal_set_message(t('Wind high: ' . $wind_hi));
//    drupal_set_message('Wind dir: ' . $wind_dir);
//    drupal_set_message(t('Rain: ' . $rain));
//    drupal_set_message('Bar: ' . $bar);
//    drupal_set_message('Arc int: ' . $arc_int);
//    drupal_set_message('Wind dir deg: ' . $wind_dir_deg);

    $sample = db_insert('bamp_weather')
        ->fields(array(
          'date' => $date,
          'time' => $time,
          'site_name' => $site_name,
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

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

/**
 * Create BAMP CSV upload form for Crawford data
 */
function bamp_upload_environment_form($form_state) {
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

function bamp_upload_environment_menu_callback() {
  return drupal_get_form('bamp_upload_environment_form');
}

function bamp_upload_environment_form_validate($form, &$form_state) {

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

function bamp_upload_environment_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ';');

  $na = (count($aa) - 2);

//  $nu = '100';

  $nu = $na;

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));
  drupal_set_message(t("@nu" . ' of these entries were uploaded', array('@nu' => $nu)));

  for ($j = 3; $j <= $nu; $j++) {

    $aaj = explode(',', $aa[$j][0]);

//    drupal_set_message($aa[$j]);

    var_dump($aaj);

    $site_name = 'unknown';
    $date_field = explode(' ', $aaj[1]);
    $date = $date_field[1];
    $temp_1 = $aaj[13];
    $oxygen_1 = $aaj[6];
    $sal_1 = $aaj[8];
    $depth_1 = '1';

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $sample = db_insert('bamp_environment')
        ->fields(array(
          'site_name' => $site_name,
          'date' => $date,
          'temp' => $temp_1,
          'oxygen' => $oxygen_1,
          'sal' => $sal_1,
          'depth' => $depth_1,
          'upload_user' => $username,
        ))
        ->execute();

    $temp_5 = $aaj[5];
    $oxygen_5 = $aaj[4];
    $sal_5 = $aaj[14];
    $depth_5 = '5';

    $sample = db_insert('bamp_environment')
        ->fields(array(
          'site_name' => $site_name,
          'date' => $date,
          'temp' => $temp_5,
          'oxygen' => $oxygen_5,
          'sal' => $sal_5,
          'depth' => $depth_5,
          'upload_user' => $username,
        ))
        ->execute();

    $temp_10 = $aaj[3];
    $oxygen_10 = $aaj[7];
    $sal_10 = $aaj[9];
    $depth_10 = '10';

    $sample = db_insert('bamp_environment')
        ->fields(array(
          'site_name' => $site_name,
          'date' => $date,
          'temp' => $temp_10,
          'oxygen' => $oxygen_10,
          'sal' => $sal_10,
          'depth' => $depth_10,
          'upload_user' => $username,
        ))
        ->execute();

    $temp_15 = $aaj[10];
    $oxygen_15 = $aaj[11];
    $sal_15 = $aaj[12];
    $depth_15 = '15';

    $sample = db_insert('bamp_environment')
        ->fields(array(
          'site_name' => $site_name,
          'date' => $date,
          'temp' => $temp_15,
          'oxygen' => $oxygen_15,
          'sal' => $sal_15,
          'depth' => $depth_15,
          'upload_user' => $username,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

/**
 * Create BAMP CSV upload form for DFO field data
 * This may not be necessary either because the data only relates to fish caught, not lice levels
 */
function dfo_upload_field_form($form_state) {
  drupal_set_title(t('DFO field CSV upload form'));

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

function dfo_upload_field_menu_callback() {
  return drupal_get_form('dfo_upload_field_form');
}

function dfo_upload_field_form_validate($form, &$form_state) {

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

function dfo_upload_field_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ',');

  $na = (count($aa) - 1);

//  $nu = '100';

  $nu = $na;

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));
  drupal_set_message(t("@nu" . ' of these entries were uploaded', array('@nu' => $nu)));

  for ($j = 1; $j <= $nu; $j++) {

    $set_id = $aa[$j][0];
    $species = $aa[$j][1];
    $size = $aa[$j][2];
    $no_caught = $aa[$j][5];
    $comments = $aa[$j][7];
    $comments = str_replace(',', ';', $comments);
    $change_log = $aa[$j][8];
    $change_log = str_replace(',', ';', $change_log);

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $sample = db_insert('bamp_field_results')
        ->fields(array(
          'fish_id' => $set_id . '-' . (int) $fish_no,
          'species' => $species,
          'comments' => $oxygen_1,
          'sal' => $sal_1,
          'depth' => $depth_1,
          'group_name' => 'DFO',
          'upload_user' => $username,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

/**
 * Create BAMP CSV upload form for DFO site data
 */
function dfo_upload_site_form($form_state) {
  drupal_set_title(t('DFO site CSV upload form'));

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

function dfo_upload_site_menu_callback() {
  return drupal_get_form('dfo_upload_site_form');
}

function dfo_upload_site_form_validate($form, &$form_state) {

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

function dfo_upload_site_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ',');

  $na = (count($aa) - 1);

//  $nu = '100';  

  $nu = $na;

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));
  drupal_set_message(t("@nu" . ' of these entries were uploaded', array('@nu' => $nu)));

  for ($j = 1; $j <= $nu; $j++) {

    $collection_period = $aa[$j][0];
    $start_date = $aa[$j][1];
    $date_time = $aa[$j][2];
    $date_array = explode(' ', $date_time);
    $date = $date_array[0];
    $time = $aa[$j][3];
    $set_id = $aa[$j][4];
    $vessel = $aa[$j][5];
    $region = $aa[$j][6];
    $site_name = $aa[$j][7];
    $area = $aa[$j][8];
    $latitude = $aa[$j][9];
    $longitude = $aa[$j][10];
    $zone = $aa[$j][11];
    $gear_type = $aa[$j][12];
    $water_temp = $aa[$j][13];
    $salinity = $aa[$j][14];
    $refractometer = $aa[$j][15];
    $comments = $aa[$j][16] . ' ' . $aa[$j][17];
    $comments = str_replace(',', ';', $comments);

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    if ($salinity == "") {
      $salinity = NULL;
    }

    if ($water_temp == "") {
      $water_temp = NULL;
    }

    if ($refractometer == "") {
      $refractometer = NULL;
    }

    $sample = db_insert('bamp_sites')
        ->fields(array(
          'name' => $site_name,
          'date' => $date,
          'min_latitude' => $latitude,
          'min_longitude' => $longitude,
          'farm_id' => $set_id,
          'zone' => $zone,
          'area' => $region . '/' . $area,
          'rep' => $collection_period,
          'group_name' => 'DFO',
          'upload_user' => $username,
        ))
        ->execute();

    $sample = db_insert('bamp_trip')
        ->fields(array(
          'name' => $site_name,
          'start_date' => $date,
          'start_time' => $time,
          'method' => $gear_type,
          'site_id' => $set_id,
          'vessel' => $vessel,
          'comments' => $comments,
          'upload_user' => $username,
        ))
        ->execute();

    $sample = db_insert('bamp_environment')
        ->fields(array(
          'site_name' => $site_name,
          'date' => $date,
          'temp' => $water_temp,
          'sal' => $salinity,
          'refractometer' => $refractometer,
          'depth' => '0',
          'upload_user' => $username,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

/**
 * Create BAMP CSV upload form for DFO lab data
 */
function dfo_upload_lab_form($form_state) {
  drupal_set_title(t('DFO lab CSV upload form'));

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

function dfo_upload_lab_menu_callback() {
  return drupal_get_form('dfo_upload_lab_form');
}

function dfo_upload_lab_form_validate($form, &$form_state) {

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

function dfo_upload_lab_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ',');

  $na = (count($aa) - 1);

//  $nu = '100';

  $nu = $na;

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));
  drupal_set_message(t("@nu" . ' of these entries were uploaded', array('@nu' => $nu)));

  for ($j = 1; $j <= $nu; $j++) {

    if ($aa[$j][0] !== '') {
      $set_id = $aa[$j][0];
    }
    else {
      $set_id = NULL;
    }
    $species = $aa[$j][1];
    $fish_no = $aa[$j][3];
    $length = $aa[$j][4];
    $weight = $aa[$j][5];

    if ($aa[$j][6] !== '') {
      $total_lep = $aa[$j][6];
    }
    else {
      $total_lep = NULL;
    }

    if ($aa[$j][7] !== '') {
      $total_cal = $aa[$j][7];
    }
    else {
      $total_cal = NULL;
    }

    if ($aa[$j][8] !== '') {
      $total_lice = $aa[$j][8];
    }
    else {
      $total_lice = NULL;
    }

    $lep_c0 = $aa[$j][9];
    $lep_c1 = $aa[$j][10];
    $lep_c2 = $aa[$j][11];
    $lep_c3 = $aa[$j][12];
    $lep_c4m = $aa[$j][13];
    $lep_c4f = $aa[$j][14];
    $total_lep_c4 = $lep_c4m + $lep_c4f;
    $lep_pam = $aa[$j][15];
    $lep_paf = $aa[$j][16];
    $lep_am = $aa[$j][17];
    $lep_af = $aa[$j][18];
    $cal_c0 = $aa[$j][19];
    $cal_c1 = $aa[$j][20];
    $cal_c2 = $aa[$j][21];
    $cal_c3 = $aa[$j][22];
    $cal_c4m = $aa[$j][23];
    $cal_c4f = $aa[$j][24];
    $total_cal_c4 = $cal_c4m + $cal_c4f;
    $cal_pam = $aa[$j][25];
    $cal_paf = $aa[$j][26];
    $cal_am = $aa[$j][27];
    $cal_af = $aa[$j][28];
    if ($aa[$j][29] !== '') {
      $lep_nm_notid = $aa[$j][29];
    }
    else {
      $lep_nm_notid = NULL;
    }

    if ($aa[$j][30] !== '') {
      $lep_mot_notid = $aa[$j][30];
    }
    else {
      $lep_mot_notid = NULL;
    }

    if ($aa[$j][31] !== '') {
      $cal_nm_notid = $aa[$j][31];
    }
    else {
      $cal_nm_notid = NULL;
    }

    if ($aa[$j][32] !== '') {
      $cal_mot_notid = $aa[$j][32];
    }
    else {
      $cal_mot_notid = NULL;
    }

    if ($aa[$j][33] !== '') {
      $condition_factor = $aa[$j][33];
    }
    else {
      $condition_factor = NULL;
    }

    $comments = $aa[$j][34];
    $comments = str_replace(',', ';', $comments);
    $change_log = $aa[$j][35];
    $change_log = str_replace(',', ';', $change_log);

    $date = db_select('bamp_trip', 't')
        ->fields('t', array('start_date'))
        ->condition('site_id', $set_id, '=')
        ->execute()
        ->fetchField();

    $fish_id = $set_id . '-' . (int) $fish_no;

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $sample = db_insert('bamp_lab_results')
        ->fields(array(
          'fish_id' => $fish_id,
          'lab' => 'DFO',
          'species' => $species,
          'date' => $date,
          'length' => $length,
          'weight' => $weight,
          'total_lep' => $total_lep,
          'total_caligus' => $total_cal,
          'total_lice' => $total_lice,
          'lep_c0' => $lep_c0,
          'lep_c1' => $lep_c1,
          'lep_c2' => $lep_c2,
          'lep_c3' => $lep_c3,
          'lep_c4m' => $lep_c4m,
          'lep_c4f' => $lep_c4f,
          'total_lep_c4' => $total_lep_c4,
          'lep_pam' => $lep_pam,
          'lep_paf' => $lep_paf,
          'lep_am' => $lep_am,
          'lep_af' => $lep_af,
          'cal_c0' => $cal_c0,
          'cal_c1' => $cal_c1,
          'cal_c2' => $cal_c2,
          'cal_c3' => $cal_c3,
          'cal_c4m' => $cal_c4m,
          'cal_c4f' => $cal_c4f,
          'total_cal_c4' => $total_cal_c4,
          'cal_pam' => $cal_pam,
          'cal_paf' => $cal_paf,
          'cal_am' => $cal_am,
          'cal_af' => $cal_af,
          'lep_nm_notid' => $lep_nm_notid,
          'lep_mot_notid' => $lep_mot_notid,
          'cal_nm_notid' => $cal_nm_notid,
          'cal_mot_notid' => $cal_mot_notid,
          'condition_factor' => $condition_factor,
          'comments' => $comments,
          'group_name' => 'DFO',
          'upload_user' => $username,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}

/**
 * Create BAMP CSV upload form for DFO trip data
 * May not be necessary as this data comes in with the site data
 */
function dfo_upload_trip_form($form_state) {
  drupal_set_title(t('DFO trip CSV upload form'));

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

function dfo_upload_trip_menu_callback() {
  return drupal_get_form('dfo_upload_trip_form');
}

function dfo_upload_trip_form_validate($form, &$form_state) {

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

function dfo_upload_trip_form_submit($form, &$form_state) {

  $csv_file_path = $form_state['values']['csv']['filepath'];
  $csv_file_name = $form_state['values']['csv']['title'];

  $aa = bamp_csv2aa($csv_file_path, ';');

  $na = (count($aa) - 2);

//  $nu = '100';

  $nu = $na;

  drupal_set_message(t('There are ' . "@na" . ' entries in this file', array('@na' => $na)));
  drupal_set_message(t("@nu" . ' of these entries were uploaded', array('@na' => $na)));

  for ($j = 3; $j <= $nu; $j++) {

    $aaj = explode(',', $aa[$j][0]);

//    drupal_set_message($aa[$j]);

    var_dump($aaj);

    $site_name = 'unknown';
    $date_field = explode(' ', $aaj[1]);
    $date = $date_field[1];
    $temp_1 = $aaj[13];
    $oxygen_1 = $aaj[6];
    $sal_1 = $aaj[8];
    $depth_1 = '1';

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $sample = db_insert('bamp_environment')
        ->fields(array(
          'site_name' => $site_name,
          'date' => $date,
          'temp' => $temp_1,
          'oxygen' => $oxygen_1,
          'sal' => $sal_1,
          'depth' => $depth_1,
          'upload_user' => $username,
        ))
        ->execute();

    $temp_5 = $aaj[5];
    $oxygen_5 = $aaj[4];
    $sal_5 = $aaj[14];
    $depth_5 = '5';

    $sample = db_insert('bamp_environment')
        ->fields(array(
          'site_name' => $site_name,
          'date' => $date,
          'temp' => $temp_5,
          'oxygen' => $oxygen_5,
          'sal' => $sal_5,
          'depth' => $depth_5,
          'upload_user' => $username,
        ))
        ->execute();

    $temp_10 = $aaj[3];
    $oxygen_10 = $aaj[7];
    $sal_10 = $aaj[9];
    $depth_10 = '10';

    $sample = db_insert('bamp_environment')
        ->fields(array(
          'site_name' => $site_name,
          'date' => $date,
          'temp' => $temp_10,
          'oxygen' => $oxygen_10,
          'sal' => $sal_10,
          'depth' => $depth_10,
          'upload_user' => $username,
        ))
        ->execute();

    $temp_15 = $aaj[10];
    $oxygen_15 = $aaj[11];
    $sal_15 = $aaj[12];
    $depth_15 = '15';

    $sample = db_insert('bamp_environment')
        ->fields(array(
          'site_name' => $site_name,
          'date' => $date,
          'temp' => $temp_15,
          'oxygen' => $oxygen_15,
          'sal' => $sal_15,
          'depth' => $depth_15,
          'upload_user' => $username,
        ))
        ->execute();
  }

  drupal_set_message(t('File ' . "@csv_file_name" . ' submitted successfully', array('@csv_file_name' => $csv_file_name)));
}