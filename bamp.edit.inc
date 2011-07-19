<?php

// $Id: bamp.edit.inc $

/**
 * @file
 * Code to create bamp edit forms
 */
include 'bamp.helper.inc';

/**
 * Create BAMP edit menu
 */
function bamp_edit_form($form_state) {
  drupal_set_title(t('Edit menu'));

  $form = array();

  $form['form'] = array(
    '#markup' => '<p> This is the BAMP edit menu</p>',
  );

  return $form;
}

function bamp_edit_menu_callback() {
  return drupal_get_form('bamp_edit_form');
}

function bamp_edit_field_form($form, &$form_state, $id) {
  
  drupal_set_title(t('Field results edit form'));

//  Clear any residual warning messages but allow info messages to be displayed
  drupal_get_messages('warning');

  if ($id == NULL) {
    $form['form'] = array(
      '#type' => 'fieldset',
      '#title' => t('Entry details'),
    );

    $form['form']['id'] = array(
      '#markup' => 'Enter ID',
    );

    $form['form']['newid'] = array(
      '#type' => 'textfield',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );

    $form['data'] = array(
      '#type' => 'hidden',
      '#value' => 'new',
    );

    return $form;
  }
  
  $form = bamp_form_edit('bamp_field_results', $id);
  
  return $form;
}

function bamp_edit_field_menu_callback($id=NULL) {
  return drupal_get_form('bamp_edit_field_form', $id);
}

function bamp_edit_field_form_submit($form, &$form_state) {
  
// Determine what happened in the previous form and display relevent data
//  if an id was entered into the id form then go to that entry
  if ($form_state['values']['data'] == 'new') {
    $id = $form_state['values']['newid'];
    drupal_goto('bamp/Edit/Field/' . $id);
  }

//  If the reset button was clicked the reload the same entry from the db
  if ($form_state['clicked_button']['#value'] == $form_state['values']['reset']) {
    $id = $form_state['values']['id'];
    drupal_goto('bamp/Edit/Field/' . $id);
  }

//  If the prev button was clicked then decrease the id and go to that entry
  if ($form_state['clicked_button']['#value'] == $form_state['values']['prev']) {
    $id = $form_state['values']['id'];
    --$id;
    drupal_goto('bamp/Edit/Field/' . $id);
  }

  //  If the next button was clicked then increase the id and go to that entry
  if ($form_state['clicked_button']['#value'] == $form_state['values']['next']) {
    $id = $form_state['values']['id'];
    ++$id;
    drupal_goto('bamp/Edit/Field/' . $id);
  }

//  If the edit button was clicked then write the new values to the db
  if ($form_state['clicked_button']['#value'] == $form_state['values']['edit']) {
    
//    Make sure that any blank fields are written to the database as NULL
    foreach ($form_state['values'] as &$arr) {
      if ($arr == '') {
        $arr = NULL;
      }
    }
    
    $id = $form_state['values']['id'];
    $fish_id = $form_state['values']['fish_id'];
    $trip_id = $form_state['values']['trip_id'];
    $sample_id = $form_state['values']['sample_id'];
    $site_name = $form_state['values']['site_name'];
    $site_no = $form_state['values']['site_no'];
    $set_no = $form_state['values']['set_no'];
    $blind_no = $form_state['values']['blind_no'];
    $to_lab = $form_state['values']['to_lab'];
    $date = $form_state['values']['date'];
    $pen = $form_state['values']['pen'];
    $input_name = $form_state['values']['input_name'];
    $route = $form_state['values']['route'];
    $rep_no = $form_state['values']['rep_no'];
    $no_sampled = $form_state['values']['no_sampled'];
    $crew = $form_state['values']['crew'];
    $species = $form_state['values']['species'];
    $height = $form_state['values']['height'];
    $length = $form_state['values']['length'];
    $weight = $form_state['values']['weight'];
    $lep_cop = $form_state['values']['lep_cop'];
    $cal_cop = $form_state['values']['cal_cop'];
    $total_cop = $form_state['values']['total_cop'];
    $chal_a = $form_state['values']['chal_a'];
    $chal_b = $form_state['values']['chal_b'];
    $total_chal = $form_state['values']['total_chal'];
    $lep_pre_male = $form_state['values']['lep_pre_male'];
    $lep_pre_female = $form_state['values']['lep_pre_female'];
    $lep_pre_unid = $form_state['values']['lep_pre_unid'];
    $total_lep_pre = $form_state['values']['total_lep_pre'];
    $lep_adult_male = $form_state['values']['lep_adult_male'];
    $lep_adult_female = $form_state['values']['lep_adult_female'];
    $lep_gravid = $form_state['values']['lep_gravid'];
    $lep_adult_unid = $form_state['values']['lep_adult_unid'];
    $total_lep_adult = $form_state['values']['total_lep_adult'];
    $total_lep = $form_state['values']['total_lep'];
    $cal_pre_male = $form_state['values']['cal_pre_male'];
    $cal_pre_female = $form_state['values']['cal_pre_female'];
    $cal_pre_unid = $form_state['values']['cal_pre_unid'];
    $total_cal_pre = $form_state['values']['total_cal_pre'];
    $cal_adult_male = $form_state['values']['cal_adult_male'];
    $cal_adult_female = $form_state['values']['cal_adult_female'];
    $cal_gravid = $form_state['values']['cal_gravid'];
    $cal_adult_unid = $form_state['values']['cal_adult_unid'];
    $total_cal_adult = $form_state['values']['total_cal_adult'];
    $total_cal = $form_state['values']['total_cal'];
    $total_motile = $form_state['values']['total_motile'];
    $total_lice = $form_state['values']['total_lice'];
    $scar_chal = $form_state['values']['scar_chal'];
    $scar_mob = $form_state['values']['scar_mob'];
    $condition_factor = $form_state['values']['condition_factor'];
    $pred_mark = $form_state['values']['pred_mark'];
    $hem = $form_state['values']['hem'];
    $mate_guard = $form_state['values']['mate_guard'];
    $pin_belly = $form_state['values']['pin_belly'];
    $tissue_damage = $form_state['values']['tissue_damage'];
    $comments = $form_state['values']['comments'];

    $new_values = $form_state['values']['id'] . ',';
    $nv = count($form_state['values']) + 1;
    $new_val = array_chunk($form_state['values'], $nv);
    $one = array_shift($new_val[0]);
    $two = array_shift($new_val[0]);
    $new_values .= implode(',', $new_val[0]);


    $old_values = $form_state['values']['old_values'];

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $entry_update = db_update('bamp_field_results')
        ->fields(array(
          'fish_id' => $fish_id,
          'trip_id' => $trip_id,
          'sample_id' => $sample_id,
          'site_name' => $site_name,
          'site_no' => $site_no,
          'set_no' => $set_no,
          'blind_no' => $blind_no,
          'to_lab' => $to_lab,
          'date' => $date,
          'pen' => $pen,
          'input_name' => $input_name,
          'route' => $route,
          'rep_no' => $rep_no,
          'no_sampled' => $no_sampled,
          'crew' => $crew,
          'species' => $species,
          'height' => $height,
          'length' => $length,
          'weight' => $weight,
          'lep_cop' => $lep_cop,
          'cal_cop' => $cal_cop,
          'total_cop' => $total_cop,
          'chal_a' => $chal_a,
          'chal_b' => $chal_b,
          'total_chal' => $total_chal,
          'lep_pre_male' => $lep_pre_male,
          'lep_pre_female' => $lep_pre_female,
          'lep_pre_unid' => $lep_pre_unid,
          'total_lep_pre' => $total_lep_pre,
          'lep_adult_male' => $lep_adult_male,
          'lep_adult_female' => $lep_adult_female,
          'lep_gravid' => $lep_gravid,
          'lep_adult_unid' => $lep_adult_unid,
          'total_lep_adult' => $total_lep_adult,
          'total_lep' => $total_lep,
          'cal_pre_male' => $cal_pre_male,
          'cal_pre_female' => $cal_pre_female,
          'cal_pre_unid' => $cal_pre_unid,
          'total_cal_pre' => $total_cal_pre,
          'cal_adult_male' => $cal_adult_male,
          'cal_adult_female' => $cal_adult_female,
          'cal_gravid' => $cal_gravid,
          'cal_adult_unid' => $cal_adult_unid,
          'total_cal_adult' => $total_cal_adult,
          'total_cal' => $total_cal,
          'total_motile' => $total_motile,
          'total_lice' => $total_lice,
          'scar_chal' => $scar_chal,
          'scar_mob' => $scar_mob,
          'condition_factor' => $condition_factor,
          'pred_mark' => $pred_mark,
          'hem' => $hem,
          'mate_guard' => $mate_guard,
          'pin_belly' => $pin_belly,
          'tissue_damage' => $tissue_damage,
          'comments' => $comments,
        ))
        ->condition('id', $id, '=')
        ->execute();

//    And write the changes to the changelog table
    $changelog = db_insert('bamp_changelog')
        ->fields(array(
          'edited_table' => 'bamp_field_results',
          'old_values' => $old_values,
          'new_values' => $new_values,
          'user' => $username,
        ))
        ->execute();

    drupal_set_message(t('Entry edited successfully!'));
    drupal_goto('bamp/Edit/Field/' . $id);
  }
}

/**
 * Form for editing individual site entries
 * @param type $form
 * @param type $form_state
 * @param type $id
 * @return string 
 */
function bamp_edit_site_form($form, &$form_state, $id) {
  
  drupal_set_title(t('Site data edit form'));

  drupal_get_messages('warning');

  if ($id == NULL) {
    $form['form'] = array(
      '#type' => 'fieldset',
      '#title' => t('Entry details'),
    );

    $form['form']['id'] = array(
      '#markup' => 'Enter ID',
    );

    $form['form']['newid'] = array(
      '#type' => 'textfield',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );

    $form['data'] = array(
      '#type' => 'hidden',
      '#value' => 'new',
    );
    
    return $form;
  }
  
  $form = bamp_form_edit('bamp_sites', $id);
  
  return $form;
}

function bamp_edit_site_menu_callback($id=NULL) {
  return drupal_get_form('bamp_edit_site_form', $id);
}

function bamp_edit_site_form_submit($form, &$form_state) {
  
  if ($form_state['values']['data'] == 'new') {
    $id = $form_state['values']['newid'];
    drupal_goto('bamp/Edit/Site/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['reset']) {
    $id = $form_state['values']['id'];
    drupal_goto('bamp/Edit/Site/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['prev']) {
    $id = $form_state['values']['id'];
    --$id;
    drupal_goto('bamp/Edit/Site/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['next']) {
    $id = $form_state['values']['id'];
    ++$id;
    drupal_goto('bamp/Edit/Site/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['edit']) {
    
    foreach ($form_state['values'] as &$arr) {
      if ($arr == '') {
        $arr = NULL;
      }
    }
    
    $id = $form_state['values']['id'];
    $craw_site_id = $form_state['values']['craw_site_id'];
    $name = $form_state['values']['name'];
    $min_latitude = $form_state['values']['min_latitude'];
    $min_longitude = $form_state['values']['min_longitude'];
    $type = $form_state['values']['type'];
    $description = $form_state['values']['description'];
    $farm_id = $form_state['values']['farm_id'];
    $zone = $form_state['values']['zone'];
    $area = $form_state['values']['area'];
    $rep = $form_state['values']['rep'];
    $route = $form_state['values']['route'];
    $distance = $form_state['values']['distance'];
    $date = $form_state['values']['date'];
    $year = $form_state['values']['year'];
    $group_name = $form_state['values']['group_name'];

    $new_values = $form_state['values']['id'] . ',';
    $nv = count($form_state) + 1;
    $new_val = array_chunk($form_state['values'], $nv);
    $one = array_shift($new_val[0]);
    $two = array_shift($new_val[0]);
    $new_values .= implode(',', $new_val[0]);

    $old_values = $form_state['values']['old_values'];

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $entry_update = db_update('bamp_sites')
        ->fields(array(
          'craw_site_id' => $craw_site_id,
          'name' => $name,
          'min_latitude' => $min_latitude,
          'min_longitude' => $min_longitude,
          'type' => $type,
          'description' => $description,
          'farm_id' => $farm_id,
          'zone' => $zone,
          'area' => $area,
          'rep' => $rep,
          'route' => $route,
          'distance' => $distance,
          'date' => $date,
          'year' => $year,
          'group_name' => $group_name,
        ))
        ->condition('id', $id, '=')
        ->execute();

    $changelog = db_insert('bamp_changelog')
        ->fields(array(
          'edited_table' => 'bamp_sites',
          'old_values' => $old_values,
          'new_values' => $new_values,
          'user' => $username,
        ))
        ->execute();

    drupal_set_message(t('Entry edited successfully!'));
    drupal_set_message(implode(',', $form_state['values']));
    drupal_goto('bamp/Edit/Site/' . $id);
  }
}

/**
 * Form for editing individual weather entries
 * @param type $form
 * @param type $form_state
 * @param type $id
 * @return string 
 */
function bamp_edit_weather_form($form, &$form_state, $id) {
  
  drupal_set_title(t('Weather data edit form'));

  drupal_get_messages('warning');

  if ($id == NULL) {
    $form['form'] = array(
      '#type' => 'fieldset',
      '#title' => t('Entry details'),
    );

    $form['form']['id'] = array(
      '#markup' => 'Enter ID',
    );

    $form['form']['newid'] = array(
      '#type' => 'textfield',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );

    $form['data'] = array(
      '#type' => 'hidden',
      '#value' => 'new',
    );

    return $form;
  }
  
  $form = bamp_form_edit('bamp_weather', $id);
  
  return $form;
}

function bamp_edit_weather_menu_callback($id=NULL) {
  return drupal_get_form('bamp_edit_weather_form', $id);
}

function bamp_edit_weather_form_submit($form, &$form_state) {
  
  if ($form_state['values']['data'] == 'new') {
    $id = $form_state['values']['newid'];
    drupal_goto('bamp/Edit/Weather/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['reset']) {
    $id = $form_state['values']['id'];
    drupal_goto('bamp/Edit/Weather/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['prev']) {
    $id = $form_state['values']['id'];
    --$id;
    drupal_goto('bamp/Edit/Weather/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['next']) {
    $id = $form_state['values']['id'];
    ++$id;
    drupal_goto('bamp/Edit/Weather/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['edit']) {
    
    foreach ($form_state['values'] as &$arr) {
      if ($arr == '') {
        $arr = NULL;
      }
    }
    
    $id = $form_state['values']['id'];
    $date = $form_state['values']['date'];
    $time = $form_state['values']['time'];
    $site_name = $form_state['values']['site_name'];
    $temp = $form_state['values']['temp'];
    $temp_hi = $form_state['values']['temp_hi'];
    $temp_low = $form_state['values']['temp_low'];
    $hum = $form_state['values']['hum'];
    $dew_pt = $form_state['values']['dew_pt'];
    $wind_speed = $form_state['values']['wind_speed'];
    $wind_hi = $form_state['values']['wind_hi'];
    $wind_dir = $form_state['values']['wind_dir'];
    $wind_dir_deg = $form_state['values']['wind_dir_deg'];
    $rain = $form_state['values']['rain'];
    $bar = $form_state['values']['bar'];
    $arc_int = $form_state['values']['arc_int'];

    $new_values = $form_state['values']['id'] . ',';
    $new_val = array_chunk($form_state['values'], 58);
    $one = array_shift($new_val[0]);
    $two = array_shift($new_val[0]);
    $new_values .= implode(',', $new_val[0]);


    $old_values = $form_state['values']['old_values'];

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $entry_update = db_update('bamp_weather')
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
          'wind_dir_deg' => $wind_dir_deg,
          'rain' => $rain,
          'bar' => $bar,
          'arc_int' => $arc_int,
        ))
        ->condition('id', $id, '=')
        ->execute();

    $changelog = db_insert('bamp_changelog')
        ->fields(array(
          'edited_table' => 'bamp_weather',
          'old_values' => $old_values,
          'new_values' => $new_values,
          'user' => $username,
        ))
        ->execute();

    drupal_set_message(t('Entry edited successfully!'));
    drupal_goto('bamp/Edit/Weather/' . $id);
  }
}

/**
 * Form for editing individual environmental entries
 * @param type $form
 * @param type $form_state
 * @param type $id
 * @return string 
 */
function bamp_edit_environment_form($form, &$form_state, $id) {
  
  drupal_set_title(t('Environment data edit form'));

  drupal_get_messages('warning');

  if ($id == NULL) {
    $form['form'] = array(
      '#type' => 'fieldset',
      '#title' => t('Entry details'),
    );

    $form['form']['id'] = array(
      '#markup' => 'Enter ID',
    );

    $form['form']['newid'] = array(
      '#type' => 'textfield',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );

    $form['data'] = array(
      '#type' => 'hidden',
      '#value' => 'new',
    );

    return $form;
  }

  $form = bamp_form_edit('bamp_environment', $id);
  
  return $form; 
}
  
function bamp_edit_environment_menu_callback($id=NULL) {
  return drupal_get_form('bamp_edit_environment_form', $id);
}

function bamp_edit_environment_form_submit($form, &$form_state) {
  
  if ($form_state['values']['data'] == 'new') {
    $id = $form_state['values']['newid'];
    drupal_goto('bamp/Edit/Environment/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['reset']) {
    $id = $form_state['values']['id'];
    drupal_goto('bamp/Edit/Environment/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['prev']) {
    $id = $form_state['values']['id'];
    --$id;
    drupal_goto('bamp/Edit/Environment/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['next']) {
    $id = $form_state['values']['id'];
    ++$id;
    drupal_goto('bamp/Edit/Environment/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['edit']) {
    
    foreach ($form_state['values'] as &$arr) {
      if ($arr == '') {
        $arr = NULL;
      }
    }
    
    $table = 'bamp_environment';
    
    $result = db_query("SHOW COLUMNS FROM $table");
    
    $headers = array();
    
    foreach ($result as $results) {
      $headers[] = $results->field;
    }
    
    drupal_set_message(implode(',', $headers));
    
    foreach ($headers as $header) {
      ${$header} = $form_state['values'][$header];
    }

    $new_values = $form_state['values']['id'] . ',';
    $new_val = array_chunk($form_state['values'], 58);
    $one = array_shift($new_val[0]);
    $two = array_shift($new_val[0]);

    $new_values .= implode(',', $new_val[0]);

    $old_values = $form_state['values']['old_values'];

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $entry_update = db_update('bamp_environment')
        ->fields(array(
          'date' => $date,
          'site_name' => $site_name,
          'temp' => $temp,
          'oxygen' => $oxygen,
          'sal' => $sal,
          'refractometer' => $refractometer,
          'depth' => $depth,
          'group_name' => $group_name,
        ))
        ->condition('id', $id, '=')
        ->execute();

    $changelog = db_insert('bamp_changelog')
        ->fields(array(
          'edited_table' => 'bamp_environment',
          'old_values' => $old_values,
          'new_values' => $new_values,
          'user' => $username,
        ))
        ->execute();

    drupal_set_message(t('Entry edited successfully!'));
    drupal_goto('bamp/Edit/Environment/' . $id);
  }
}

/**
 * Form to edit individual lab results
 * @param type $form
 * @param type $form_state
 * @param type $id
 * @return string 
 */
function bamp_edit_lab_form($form, &$form_state, $id) {
  
  drupal_set_title(t('Lab results edit form'));

  drupal_get_messages('warning');

  if ($id == NULL) {
    $form['form'] = array(
      '#type' => 'fieldset',
      '#title' => t('Entry details'),
    );

    $form['form']['id'] = array(
      '#markup' => 'Enter ID',
    );

    $form['form']['newid'] = array(
      '#type' => 'textfield',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );

    $form['data'] = array(
      '#type' => 'hidden',
      '#value' => 'new',
    );

    return $form;
  }
  
  $form = bamp_form_edit('bamp_lab_results', $id);
  
  return $form;
}

function bamp_edit_lab_menu_callback($id=NULL) {
  return drupal_get_form('bamp_edit_lab_form', $id);
}

function bamp_edit_lab_form_submit($form, &$form_state) {
  
  if ($form_state['values']['data'] == 'new') {
    $id = $form_state['values']['newid'];
    drupal_goto('bamp/Edit/Lab/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['reset']) {
    $id = $form_state['values']['id'];
    drupal_goto('bamp/Edit/Lab/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['prev']) {
    $id = $form_state['values']['id'];
    --$id;
    drupal_goto('bamp/Edit/Lab/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['next']) {
    $id = $form_state['values']['id'];
    ++$id;
    drupal_goto('bamp/Edit/Lab/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['edit']) {
    
    foreach ($form_state['values'] as &$arr) {
      if ($arr == '') {
        $arr = NULL;
      }
    }
    
    $id = $form_state['values']['id'];
    $fish_id = $form_state['values']['fish_id'];
    $date = $form_state['values']['date'];
    $lab_id = $form_state['values']['lab_id'];
    $species = $form_state['values']['species'];
    $method = $form_state['values']['method'];
    $people = $form_state['values']['people'];
    $length = $form_state['values']['length'];
    $weight = $form_state['values']['weight'];
    $total_lep = $form_state['values']['total_lep'];
    $total_caligus = $form_state['values']['total_caligus'];
    $total_lice = $form_state['values']['total_lice'];
    $lep_cop = $form_state['values']['lep_cop'];
    $lep_c1 = $form_state['values']['lep_c1'];
    $lep_c2 = $form_state['values']['lep_c2'];
    $lep_c3 = $form_state['values']['lep_c3'];
    $lep_c4m = $form_state['values']['lep_c4m'];
    $lep_c4f = $form_state['values']['lep_c4f'];
    $lep_pam = $form_state['values']['lep_pam'];
    $lep_paf = $form_state['values']['lep_paf'];
    $lep_am = $form_state['values']['lep_am'];
    $lep_af = $form_state['values']['lep_af'];
    $cal_cop = $form_state['values']['cal_cop'];
    $cal_c1 = $form_state['values']['cal_c1'];
    $cal_c2 = $form_state['values']['cal_c2'];
    $cal_c3 = $form_state['values']['cal_c3'];
    $cal_c4m = $form_state['values']['cal_c4m'];
    $cal_c4f = $form_state['values']['cal_c4f'];
    $cal_pam = $form_state['values']['cal_pam'];
    $cal_paf = $form_state['values']['cal_paf'];
    $cal_am = $form_state['values']['cal_am'];
    $cal_af = $form_state['values']['cal_af'];
    $lep_nm_notid = $form_state['values']['lep_nm_notid'];
    $lep_mot_notid = $form_state['values']['lep_mot_notid'];
    $cal_nm_notid = $form_state['values']['cal_nm_notid'];
    $cal_mot_notid = $form_state['values']['cal_mot_notid'];
    $condition_factor = $form_state['values']['condition_factor'];
    $scarring = $form_state['values']['scarring'];
    $comments = $form_state['values']['comments'];

    $new_values = $form_state['values']['id'] . ',';
    $new_val = array_chunk($form_state['values'], 58);
    $one = array_shift($new_val[0]);
    $two = array_shift($new_val[0]);
    $new_values .= implode(',', $new_val[0]);


    $old_values = $form_state['values']['old_values'];

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $entry_update = db_update('bamp_lab_results')
        ->fields(array(
          'fish_id' => $fish_id,
          'date' => $date,
          'lab_id' => $lab_id,
          'species' => $species,
          'method' => $method,
          'people' => $people,
          'length' => $length,
          'weight' => $weight,
          'total_lep' => (int) $total_lep,
          'total_caligus' => (int) $total_caligus,
          'total_lice' => (int) $total_lice,
          'lep_cop' => (int) $lep_cop,
          'lep_c1' => (int) $lep_c1,
          'lep_c2' => (int) $lep_c2,
          'lep_c3' => (int) $lep_c3,
          'lep_c4m' => (int) $lep_c4m,
          'lep_c4f' => (int) $lep_c4f,
          'lep_pam' => (int) $lep_pam,
          'lep_paf' => (int) $lep_paf,
          'lep_am' => (int) $lep_am,
          'lep_af' => (int) $lep_af,
          'cal_cop' => (int) $cal_cop,
          'cal_c1' => (int) $cal_c1,
          'cal_c2' => (int) $cal_c2,
          'cal_c3' => (int) $cal_c3,
          'cal_c4m' => (int) $cal_c4m,
          'cal_c4f' => (int) $cal_c4f,
          'cal_pam' => (int) $cal_pam,
          'cal_paf' => (int) $cal_paf,
          'cal_am' => (int) $cal_am,
          'cal_af' => (int) $cal_af,
          'lep_nm_notid' => (int) $lep_nm_notid,
          'lep_mot_notid' => (int) $lep_mot_notid,
          'cal_nm_notid' => (int) $cal_nm_notid,
          'cal_mot_notid' => (int) $cal_mot_notid,
          'condition_factor' => (float) $condition_factor,
          'scarring' => $scarring,
          'comments' => $comments,
        ))
        ->condition('id', $id, '=')
        ->execute();

    $changelog = db_insert('bamp_changelog')
        ->fields(array(
          'edited_table' => 'bamp_lab_results',
          'old_values' => $old_values,
          'new_values' => $new_values,
          'user' => $username,
        ))
        ->execute();

    drupal_set_message(t('Entry edited successfully!'));
    drupal_goto('bamp/Edit/Lab/' . $id);
  }
}

/**
 * Form for editing individual trip entries
 * @param type $form
 * @param type $form_state
 * @param type $id
 * @return string 
 */
function bamp_edit_trip_form($form, &$form_state, $id) {
  drupal_set_title(t('Trip data edit form'));

  drupal_get_messages('warning');

  if ($id == NULL) {
    $form['form'] = array(
      '#type' => 'fieldset',
      '#title' => t('Entry details'),
    );

    $form['form']['id'] = array(
      '#markup' => 'Enter ID',
    );

    $form['form']['newid'] = array(
      '#type' => 'textfield',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );

    $form['data'] = array(
      '#type' => 'hidden',
      '#value' => 'new',
    );

    return $form;
  }

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entry details'),
  );

  $select = db_select('bamp_trip', 't');

  $select->condition('t.id', $id, '=');

  $select->addField('t', 'id');
  $select->addField('t', 'start_date');
  $select->addField('t', 'start_time');
  $select->addField('t', 'site_id');
  $select->addField('t', 'name');
  $select->addField('t', 'vessel');
  $select->addField('t', 'method');
  $select->addField('t', 'comments');
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  if ($entries == NULL) {
    drupal_set_message(t('Entry not found!'), 'warning');

    $form['form']['id'] = array(
      '#markup' => 'Id: ' . $id . ' ',
    );

    $form['form']['next'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
    );

    $form['id'] = array(
      '#type' => 'hidden',
      '#value' => $id,
    );

    $form['prev'] = array(
      '#type' => 'hidden',
      '#value' => 'prev',
    );

    $form['reset'] = array(
      '#type' => 'hidden',
      '#value' => 'reset',
    );

    $form['data'] = array(
      '#type' => 'hidden',
      '#value' => 'reset',
    );

    return $form;
  }

  $old_values = implode(',', $entries[0]);

//  Assign the data to variables casting them as the proper type so they go back in to the database properly 
  $entry_id = (int) $entries[0]['id'];
  $start_date = $entries[0]['start_date'];
  $start_time = $entries[0]['start_time'];
  $site_id = $entries[0]['site_id'];
  $name = $entries[0]['name'];
  $vessel = $entries[0]['vessel'];
  $method = $entries[0]['method'];
  $comments = $entries[0]['comments'];

  $form['form']['prev'] = array(
    '#type' => 'submit',
    '#value' => t('Prev'),
  );

  $form['form']['id'] = array(
    '#markup' => 'Id: ' . $entry_id,
  );

  $form['form']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  $form['form']['start_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#default_value' => $start_date,
  );

  $form['form']['start_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Time'),
    '#default_value' => $start_time,
  );

  $form['form']['site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#default_value' => $site_id,
  );

  $form['form']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Temp'),
    '#default_value' => $name,
  );

  $form['form']['vessel'] = array(
    '#type' => 'textfield',
    '#title' => t('Vessel'),
    '#default_value' => $vessel,
  );

  $form['form']['method'] = array(
    '#type' => 'textfield',
    '#title' => t('Method'),
    '#default_value' => $method,
  );

  $form['form']['comments'] = array(
    '#type' => 'textfield',
    '#title' => t('Comments'),
    '#default_value' => $comments,
  );

  $form['edit'] = array(
    '#type' => 'submit',
    '#value' => 'Edit',
  );

  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => 'Reset',
  );

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $id,
  );

  $form['old_values'] = array(
    '#type' => 'hidden',
    '#value' => $old_values,
  );

  $form['data'] = array(
    '#type' => 'hidden',
    '#value' => 'reset',
  );

  return $form;
}

function bamp_edit_trip_menu_callback($id=NULL) {
  return drupal_get_form('bamp_edit_trip_form', $id);
}

function bamp_edit_trip_form_submit($form, &$form_state) {
  if ($form_state['values']['data'] == 'new') {
    $id = $form_state['values']['newid'];
    drupal_goto('bamp/Edit/Trip/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['reset']) {
    $id = $form_state['values']['id'];
    drupal_goto('bamp/Edit/Trip/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['prev']) {
    $id = $form_state['values']['id'];
    --$id;
    drupal_goto('bamp/Edit/Trip/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['next']) {
    $id = $form_state['values']['id'];
    ++$id;
    drupal_goto('bamp/Edit/Trip/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['edit']) {
    
    foreach ($form_state['values'] as &$arr) {
      if ($arr == '') {
        $arr = NULL;
      }
    }
    
    $id = $form_state['values']['id'];
    $start_date = $form_state['values']['start_date'];
    $start_time = $form_state['values']['start_time'];
    $site_id = $form_state['values']['site_id'];
    $name = $form_state['values']['name'];
    $vessel = $form_state['values']['vessel'];
    $method = $form_state['values']['method'];
    $comments = $form_state['values']['comments'];

    $new_values = $form_state['values']['id'] . ',';
    $new_val = array_chunk($form_state['values'], 58);
    $one = array_shift($new_val[0]);
    $two = array_shift($new_val[0]);

    $new_values .= implode(',', $new_val[0]);

    $old_values = $form_state['values']['old_values'];

    $uid = user_uid_optional_to_arg('%');
    $username = bamp_user_name($uid);

    $entry_update = db_update('bamp_trip')
        ->fields(array(
          'start_date' => $start_date,
          'start_time' => $start_time,
          'site_id' => $site_id,
          'name' => $name,
          'vessel' => $vessel,
          'method' => $method,
          'comments' => $comments,
        ))
        ->condition('id', $id, '=')
        ->execute();

    $changelog = db_insert('bamp_changelog')
        ->fields(array(
          'edited_table' => 'bamp_trip',
          'old_values' => $old_values,
          'new_values' => $new_values,
          'user' => $username,
        ))
        ->execute();

    drupal_set_message(t('Entry edited successfully!'));
    drupal_goto('bamp/Edit/Trip/' . $id);
  }
}