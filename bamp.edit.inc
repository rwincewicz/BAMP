<?php

// $Id: bamp.edit.inc $

/**
 * @file
 * Code to create bamp edit forms
 */

include 'bamp.helper.inc';

/**
 * Create BAMP report menu
 */
function bampedit_form($form_state) {
  drupal_set_title(t('Edit menu'));

  $form = array();

  $content['form'] = array(
    '#markup' => '<p> This is the BAMP edit menu</p>',
  );

  return $form;
}

function bampedit_menu_callback() {
  return drupal_get_form('bampedit_form');
}

function bampeditfield_form($form, &$form_state, $id) {
  drupal_set_title(t('Field results edit form'));

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

  $select = db_select('bamp_field_results', 'f');

  $select->condition('f.id', $id, '=');

  $select->addField('f', 'id');
  $select->addField('f', 'fish_id');
  $select->addField('f', 'trip_id');
  $select->addField('f', 'sample_id');
  $select->addField('f', 'site_name');
  $select->addField('f', 'site_no');
  $select->addField('f', 'set_no');
  $select->addField('f', 'blind_no');
  $select->addField('f', 'to_lab');
  $select->addField('f', 'date');
  $select->addField('f', 'pen');
  $select->addField('f', 'input_name');
  $select->addField('f', 'route');
  $select->addField('f', 'rep_no');
  $select->addField('f', 'no_sampled');
  $select->addField('f', 'crew');
  $select->addField('f', 'species');
  $select->addField('f', 'height');
  $select->addField('f', 'length');
  $select->addField('f', 'weight');
  $select->addField('f', 'cop_lep');
  $select->addField('f', 'cop_cal');
  $select->addField('f', 'total_cop');
  $select->addField('f', 'chal_a');
  $select->addField('f', 'chal_b');
  $select->addField('f', 'total_chal');
  $select->addField('f', 'lep_pre_male');
  $select->addField('f', 'lep_pre_female');
  $select->addField('f', 'lep_pre_unid');
  $select->addField('f', 'total_lep_pre');
  $select->addField('f', 'lep_adult_male');
  $select->addField('f', 'lep_adult_female');
  $select->addField('f', 'lep_gravid');
  $select->addField('f', 'lep_adult_unid');
  $select->addField('f', 'total_lep_adult');
  $select->addField('f', 'total_lep');
  $select->addField('f', 'cal_pre_male');
  $select->addField('f', 'cal_pre_female');
  $select->addField('f', 'cal_pre_unid');
  $select->addField('f', 'total_cal_pre');
  $select->addField('f', 'cal_adult_male');
  $select->addField('f', 'cal_adult_female');
  $select->addField('f', 'cal_gravid');
  $select->addField('f', 'cal_adult_unid');
  $select->addField('f', 'total_cal_adult');
  $select->addField('f', 'total_cal');
  $select->addField('f', 'total_motile');
  $select->addField('f', 'total_lice');
  $select->addField('f', 'scar_chal');
  $select->addField('f', 'scar_mob');
  $select->addField('f', 'condition_factor');
  $select->addField('f', 'pred_mark');
  $select->addField('f', 'hem');
  $select->addField('f', 'mate_guard');
  $select->addField('f', 'pin_belly');
  $select->addField('f', 'tissue_damage');
  $select->addField('f', 'comments');
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  $old_values = implode(',', $entries[0]);
  
//  Assign the data to variables casting them as the proper type so they go back in to the database properly 
  $entry_id = (int) $entries[0]['id'];
  $fish_id = $entries[0]['fish_id'];
  $trip_id = (int) $entries[0]['trip_id'];
  $sample_id = (int) $entries[0]['sample_id'];
  $site_name = $entries[0]['site_name'];
  $site_no = (int) $entries[0]['site_no'];
  $set_no = (int) $entries[0]['set_no'];
  $blind_no = (int) $entries[0]['blind_no'];
  $to_lab = $entries[0]['to_lab'];
  $date = $entries[0]['date'];
  $pen = (int) $entries[0]['pen'];
  $input_name = $entries[0]['input_name'];
  $route = $entries[0]['route'];
  $rep_no = (int) $entries[0]['rep_no'];
  $no_sampled = (int) $entries[0]['no_sampled'];
  $crew = $entries[0]['crew'];
  $species = $entries[0]['species'];
  $height = (float) $entries[0]['height'];
  $length = (float) $entries[0]['length'];
  $weight = (float) $entries[0]['weight'];
  $cop_lep = (int) $entries[0]['cop_lep'];
  $cop_cal = (int) $entries[0]['cop_cal'];
  $total_cop = (int) $entries[0]['total_cop'];
  $chal_a = (int) $entries[0]['chal_a'];
  $chal_b = (int) $entries[0]['chal_b'];
  $total_chal = (int) $entries[0]['total_chal'];
  $lep_pre_male = (int) $entries[0]['lep_pre_male'];
  $lep_pre_female = (int) $entries[0]['lep_pre_female'];
  $lep_pre_unid = (int) $entries[0]['lep_pre_unid'];
  $total_lep_pre = (int) $entries[0]['total_lep_pre'];
  $lep_adult_male = (int) $entries[0]['lep_adult_male'];
  $lep_adult_female = (int) $entries[0]['lep_adult_female'];
  $lep_gravid = (int) $entries[0]['lep_gravid'];
  $lep_adult_unid = (int) $entries[0]['lep_adult_unid'];
  $total_lep_adult = (int) $entries[0]['total_lep_adult'];
  $total_lep = (int) $entries[0]['total_lep'];
  $cal_pre_male = (int) $entries[0]['cal_pre_male'];
  $cal_pre_female = (int) $entries[0]['cal_pre_female'];
  $cal_pre_unid = (int) $entries[0]['cal_pre_unid'];
  $total_cal_pre = (int) $entries[0]['total_cal_pre'];
  $cal_adult_male = (int) $entries[0]['cal_adult_male'];
  $cal_adult_female = (int) $entries[0]['cal_adult_female'];
  $cal_gravid = (int) $entries[0]['cal_gravid'];
  $cal_adult_unid = (int) $entries[0]['cal_adult_unid'];
  $total_cal_adult = (int) $entries[0]['total_cal_adult'];
  $total_cal = (int) $entries[0]['total_cal'];
  $total_motile = (int) $entries[0]['total_motile'];
  $total_lice = (int) $entries[0]['total_lice'];
  $scar_chal = (int) $entries[0]['scar_chal'];
  $scar_mob = (int) $entries[0]['scar_mob'];
  $condition_factor = (float) $entries[0]['condition_factor'];
  $pred_mark = (float) $entries[0]['pred_mark'];
  $hem = $entries[0]['hem'];
  $mate_guard = $entries[0]['mate_guard'];
  $pin_belly = $entries[0]['pin_belly'];
  $tissue_damage = $entries[0]['tissue_damage'];
  $comments = $entries[0]['comments'];

  $form['form']['id'] = array(
    '#markup' => 'Id: ' . $entry_id,
  );

  $form['form']['fish_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Fish ID'),
    '#default_value' => $fish_id,
  );

  $form['form']['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#default_value' => $trip_id,
  );

  $form['form']['sample_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Sample ID'),
    '#default_value' => $sample_id,
  );

  $form['form']['site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Name'),
    '#default_value' => $site_name,
  );

  $form['form']['site_no'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Number'),
    '#default_value' => $site_no,
  );

  $form['form']['set_no'] = array(
    '#type' => 'textfield',
    '#title' => t('Set no'),
    '#default_value' => $set_no,
  );

  $form['form']['blind_no'] = array(
    '#type' => 'textfield',
    '#title' => t('Blind no'),
    '#default_value' => $blind_no,
  );

  $form['form']['to_lab'] = array(
    '#type' => 'textfield',
    '#title' => t('To Lab'),
    '#default_value' => $to_lab,
  );

  $form['form']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Date'),
    '#default_value' => $date,
  );

  $form['form']['pen'] = array(
    '#type' => 'textfield',
    '#title' => t('Pen'),
    '#default_value' => $pen,
  );

  $form['form']['input_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Input Name'),
    '#default_value' => $input_name,
  );

  $form['form']['route'] = array(
    '#type' => 'textfield',
    '#title' => t('Route'),
    '#default_value' => $route,
  );

  $form['form']['rep_no'] = array(
    '#type' => 'textfield',
    '#title' => t('Repetition no'),
    '#default_value' => $rep_no,
  );

  $form['form']['no_sampled'] = array(
    '#type' => 'textfield',
    '#title' => t('Number Sampled'),
    '#default_value' => $no_sampled,
  );

  $form['form']['crew'] = array(
    '#type' => 'textfield',
    '#title' => t('Crew'),
    '#default_value' => $crew,
  );

  $form['form']['species'] = array(
    '#type' => 'textfield',
    '#title' => t('Species'),
    '#default_value' => $species,
  );

  $form['form']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height/mm'),
    '#default_value' => $height,
  );

  $form['form']['length'] = array(
    '#type' => 'textfield',
    '#title' => t('Length/mm'),
    '#default_value' => $length,
  );

  $form['form']['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight/g'),
    '#default_value' => $weight,
  );

  $form['form']['cop_lep'] = array(
    '#type' => 'textfield',
    '#title' => t('Copepods Lep'),
    '#default_value' => $cop_lep,
  );

  $form['form']['cop_cal'] = array(
    '#type' => 'textfield',
    '#title' => t('Copepods Caligus'),
    '#default_value' => $cop_cal,
  );

  $form['form']['total_cop'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Copepods'),
    '#default_value' => $total_cop,
  );

  $form['form']['chal_a'] = array(
    '#type' => 'textfield',
    '#title' => t('Chalimus a'),
    '#default_value' => $chal_a,
  );

  $form['form']['chal_b'] = array(
    '#type' => 'textfield',
    '#title' => t('Chalimus b'),
    '#default_value' => $chal_b,
  );

  $form['form']['total_chal'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Chalimus'),
    '#default_value' => $total_chal,
  );

  $form['form']['lep_pre_male'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Male'),
    '#default_value' => $lep_pre_male,
  );

  $form['form']['lep_pre_female'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Female'),
    '#default_value' => $lep_pre_female,
  );

  $form['form']['lep_pre_unid'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Unidentified'),
    '#default_value' => $lep_pre_unid,
  );

  $form['form']['total_lep_pre'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Lep Pre-Adult'),
    '#default_value' => $total_lep_pre,
  );

  $form['form']['lep_adult_male'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Adult Male'),
    '#default_value' => $lep_adult_male,
  );

  $form['form']['lep_adult_female'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Adult Female'),
    '#default_value' => $lep_adult_female,
  );

  $form['form']['lep_gravid'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Gravid'),
    '#default_value' => $lep_gravid,
  );

  $form['form']['lep_adult_unid'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Adult Unidentified'),
    '#default_value' => $lep_adult_unid,
  );

  $form['form']['total_lep_adult'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Lep Adult'),
    '#default_value' => $total_lep_adult,
  );

  $form['form']['total_lep'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Lep'),
    '#default_value' => $total_lep,
  );

  $form['form']['cal_pre_male'] = array(
    '#type' => 'textfield',
    '#title' => t('Cal Pre-Adult Male'),
    '#default_value' => $cal_pre_male,
  );

  $form['form']['cal_pre_female'] = array(
    '#type' => 'textfield',
    '#title' => t('Cal Pre-Adult Female'),
    '#default_value' => $cal_pre_female,
  );

  $form['form']['cal_pre_unid'] = array(
    '#type' => 'textfield',
    '#title' => t('Cal Pre-Adult Unidentified'),
    '#default_value' => $cal_pre_unid,
  );

  $form['form']['total_cal_pre'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Cal Pre-Adult'),
    '#default_value' => $total_cal_pre,
  );

  $form['form']['cal_adult_male'] = array(
    '#type' => 'textfield',
    '#title' => t('Cal Adult Male'),
    '#default_value' => $cal_adult_male,
  );

  $form['form']['cal_adult_female'] = array(
    '#type' => 'textfield',
    '#title' => t('Cal Adult Female'),
    '#default_value' => $cal_adult_female,
  );

  $form['form']['cal_gravid'] = array(
    '#type' => 'textfield',
    '#title' => t('Cal Gravid'),
    '#default_value' => $cal_gravid,
  );

  $form['form']['cal_adult_unid'] = array(
    '#type' => 'textfield',
    '#title' => t('Cal Adult Unidentified'),
    '#default_value' => $cal_adult_unid,
  );

  $form['form']['total_cal_adult'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Cal Adult'),
    '#default_value' => $total_cal_adult,
  );

  $form['form']['total_cal'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Cal'),
    '#default_value' => $total_cal,
  );

  $form['form']['lep_pre_male'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Male'),
    '#default_value' => $lep_pre_male,
  );

  $form['form']['lep_pre_female'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Female'),
    '#default_value' => $lep_pre_female,
  );

  $form['form']['lep_pre_unid'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Unidentified'),
    '#default_value' => $lep_pre_unid,
  );

  $form['form']['total_lep_pre'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Lep Pre-Adult'),
    '#default_value' => $total_lep_pre,
  );

  $form['form']['lep_adult_male'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Adult Male'),
    '#default_value' => $lep_adult_male,
  );

  $form['form']['lep_adult_female'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Adult Female'),
    '#default_value' => $lep_adult_female,
  );

  $form['form']['lep_gravid'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Gravid'),
    '#default_value' => $lep_gravid,
  );

  $form['form']['lep_adult_unid'] = array(
    '#type' => 'textfield',
    '#title' => t('Lep Adult Unidentified'),
    '#default_value' => $lep_adult_unid,
  );

  $form['form']['total_lep_adult'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Lep Adult'),
    '#default_value' => $total_lep_adult,
  );

  $form['form']['total_lep'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Lep'),
    '#default_value' => $total_lep,
  );

  $form['form']['total_motile'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Motile'),
    '#default_value' => $total_motile,
  );

  $form['form']['total_lice'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Lice'),
    '#default_value' => $total_lice,
  );

  $form['form']['scar_chal'] = array(
    '#type' => 'textfield',
    '#title' => t('Scar - Chalimus'),
    '#default_value' => $scar_chal,
  );

  $form['form']['scar_mob'] = array(
    '#type' => 'textfield',
    '#title' => t('Scar - Mobile'),
    '#default_value' => $scar_mob,
  );
  
  $form['form']['condition_factor'] = array(
    '#type' => 'textfield',
    '#title' => t('Condition Factor'),
    '#default_value' => $condition_factor,
  );

  $form['form']['pred_mark'] = array(
    '#type' => 'textfield',
    '#title' => t('Predatory Marks'),
    '#default_value' => $pred_mark,
  );

  $form['form']['hem'] = array(
    '#type' => 'textfield',
    '#title' => t('Bleeding'),
    '#default_value' => $hem,
  );

  $form['form']['mate_guard'] = array(
    '#type' => 'textfield',
    '#title' => t('Mate Guard'),
    '#default_value' => $mate_guard,
  );

  $form['form']['pin_belly'] = array(
    '#type' => 'textfield',
    '#title' => t('Pinch Belly'),
    '#default_value' => $pin_belly,
  );

  $form['form']['tissue_damage'] = array(
    '#type' => 'textfield',
    '#title' => t('Tissue Damage'),
    '#default_value' => $tissue_damage,
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

function bampeditfield_menu_callback($id=NULL) {
  return drupal_get_form('bampeditfield_form', $id);
}

function bampeditfield_form_submit($form, &$form_state) {
  if ($form_state['values']['data'] == 'new') {
    $id = $form_state['values']['newid'];
    drupal_goto('bamp/Edit forms/Individual Field results/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['reset']) {
    $id = $form_state['values']['id'];
    drupal_goto('bamp/Edit forms/Individual Field results/' . $id);
  }

  if ($form_state['clicked_button']['#value'] == $form_state['values']['edit']) {
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
    $cop_lep = $form_state['values']['cop_lep'];
    $cop_cal = $form_state['values']['cop_cal'];
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
    $new_val = array_chunk($form_state['values'], 56);
    $new_values .= implode(',', $new_val[0]);
    
    
    $old_values = $form_state['values']['old_values'];
   
    $uid = user_uid_optional_to_arg('%');
    $username = user_name($uid);

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
          'cop_lep' => $cop_lep,
          'cop_cal' => $cop_cal,
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
    
    $audit = db_insert('bamp_audit')
        ->fields(array(
          'edited_table' => 'bamp_field_results',
          'old_values' => $old_values,
          'new_values' => $new_values,
          'user' => $username,
        ))
        ->execute();
    
    drupal_set_message(t('Entry edited successfully!'));
    drupal_goto('bamp/Edit forms/Individual Field results/' . $id);
  }
}