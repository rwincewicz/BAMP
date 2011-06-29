<?php


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
    $form['form']['id'] = array(
    '#markup' => '<h2>No ID entered!</h2>',
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
  $select->addField('f', 'pred_mark');
  $select->addField('f', 'hem');
  $select->addField('f', 'mate_guard');
  $select->addField('f', 'pin_belly');
  $select->addField('f', 'tissue_damage');
  $select->addField('f', 'comments');
  $entries = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  $entry_id = $entries[0]['id'];
  $fish_id = $entries[0]['fish_id'];
  $trip_id = $entries[0]['trip_id'];
  $sample_id = $entries[0]['sample_id'];
  $site_name = $entries[0]['site_name'];
  $site_no = $entries[0]['site_no'];
  $set_no = $entries[0]['set_no'];
  $blind_no = $entries[0]['blind_no'];
  $lab = $entries[0]['to_lab'];
  $date = $entries[0]['date'];
  $pen = $entries[0]['pen'];
  $input_name = $entries[0]['input_name'];
  $route = $entries[0]['route'];
  $rep_no = $entries[0]['rep_no'];
  $no_sampled = $entries[0]['no_sampled'];
  $crew = $entries[0]['crew'];
  $species = $entries[0]['species'];
  $height = $entries[0]['height'];
  $length = $entries[0]['length'];
  $weight = $entries[0]['weight'];
  $cop_lep = $entries[0]['cop_lep'];
  $cop_cal = $entries[0]['cop_cal'];
  $total_cop = $entries[0]['total_cop'];
  $chal_a = $entries[0]['chal_a'];
  $chal_b = $entries[0]['chal_b'];
  $total_chal = $entries[0]['total_chal'];
  $lep_pre_male = $entries[0]['lep_pre_male'];
  $lep_pre_female = $entries[0]['lep_pre_female'];
  $lep_pre_unid = $entries[0]['lep_pre_unid'];
  $total_lep_pre = $entries[0]['total_lep_pre'];
  $lep_adult_male = $entries[0]['lep_adult_male'];
  $lep_adult_female = $entries[0]['lep_adult_female'];
  $lep_gravid = $entries[0]['lep_gravid'];
  $lep_adult_unid = $entries[0]['lep_adult_unid'];
  $total_lep_adult = $entries[0]['total_lep_adult'];
  $total_lep = $entries[0]['total_lep'];
  $cal_pre_male = $entries[0]['cal_pre_male'];
  $cal_pre_female = $entries[0]['cal_pre_female'];
  $cal_pre_unid = $entries[0]['cal_pre_unid'];
  $total_cal_pre = $entries[0]['total_cal_pre'];
  $cal_adult_male = $entries[0]['cal_adult_male'];
  $cal_adult_female = $entries[0]['cal_adult_female'];
  $cal_gravid = $entries[0]['cal_gravid'];
  $cal_adult_unid = $entries[0]['cal_adult_unid'];
  $total_cal_adult = $entries[0]['total_cal_adult'];
  $total_cal = $entries[0]['total_cal'];
  $total_motile = $entries[0]['total_motile'];
  $total_lice = $entries[0]['total_lice'];
  $scar_chal = $entries[0]['scar_chal'];
  $scar_mob = $entries[0]['scar_mob'];
  $pred_mark = $entries[0]['pred_mark'];
  $hem = $entries[0]['hem'];
  $mate_guard = $entries[0]['mate_guard'];
  $pin_belly = $entries[0]['pin_belly'];
  $tissue_damage = $entries[0]['tissue_damage'];
  $comments = $entries[0]['comments'];
  
  $form['form']['id'] = array(
    '#markup' => 'Id: ' . $entry_id,
  );
  
  $form['form']['fishid'] = array(
  '#type' => 'textfield',
    '#title' => t('Fish ID'),
    '#default_value' => $fish_id,
  );
  
  $form['form']['tripid'] = array(
  '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#default_value' => $trip_id,
  );
  
  $form['form']['sampleid'] = array(
  '#type' => 'textfield',
    '#title' => t('Sample ID'),
    '#default_value' => $sample_id,
  );
  
  $form['form']['sitename'] = array(
  '#type' => 'textfield',
    '#title' => t('Site Name'),
    '#default_value' => $site_name,
  );
  
  $form['form']['siteno'] = array(
  '#type' => 'textfield',
    '#title' => t('Site Number'),
    '#default_value' => $site_no,
  );
  
  $form['form']['setno'] = array(
  '#type' => 'textfield',
    '#title' => t('Set no'),
    '#default_value' => $set_no,
  );
  
  $form['form']['blindno'] = array(
  '#type' => 'textfield',
    '#title' => t('Blind no'),
    '#default_value' => $blind_no,
  );
  
  $form['form']['tolab'] = array(
  '#type' => 'textfield',
    '#title' => t('To Lab'),
    '#default_value' => $lab,
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
  
  $form['form']['inputname'] = array(
  '#type' => 'textfield',
    '#title' => t('Input Name'),
    '#default_value' => $input_name,
  );
  
  $form['form']['route'] = array(
  '#type' => 'textfield',
    '#title' => t('Route'),
    '#default_value' => $route,
  );
  
  $form['form']['repno'] = array(
  '#type' => 'textfield',
    '#title' => t('Repetition no'),
    '#default_value' => $rep_no,
  );
  
  $form['form']['nosampled'] = array(
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
  
  $form['form']['coplep'] = array(
  '#type' => 'textfield',
    '#title' => t('Copepods Lep'),
    '#default_value' => $cop_lep,
  );
  
  $form['form']['copcal'] = array(
  '#type' => 'textfield',
    '#title' => t('Copepods Caligus'),
    '#default_value' => $cop_cal,
  );
  
  $form['form']['totalcop'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Copepods'),
    '#default_value' => $total_cop,
  );
  
  $form['form']['chala'] = array(
  '#type' => 'textfield',
    '#title' => t('Chalimus a'),
    '#default_value' => $chal_a,
  );
  
  $form['form']['chalb'] = array(
  '#type' => 'textfield',
    '#title' => t('Chalimus b'),
    '#default_value' => $chal_b,
  );
  
  $form['form']['totalchal'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Chalimus'),
    '#default_value' => $total_chal,
  );
  
  $form['form']['leppremale'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Male'),
    '#default_value' => $lep_pre_male,
  );
  
  $form['form']['lepprefemale'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Female'),
    '#default_value' => $lep_pre_female,
  );
  
  $form['form']['leppreunid'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Unidentified'),
    '#default_value' => $lep_pre_unid,
  );
  
  $form['form']['totalleppre'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Lep Pre-Adult'),
    '#default_value' => $total_lep_pre,
  );
  
  $form['form']['lepadultmale'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Adult Male'),
    '#default_value' => $lep_adult_male,
  );
  
  $form['form']['lepadultfemale'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Adult Female'),
    '#default_value' => $lep_adult_female,
  );
  
  $form['form']['lepgravid'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Gravid'),
    '#default_value' => $lep_gravid,
  );
  
  $form['form']['lepadultunid'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Adult Unidentified'),
    '#default_value' => $lep_adult_unid,
  );
  
  $form['form']['totallepadult'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Lep Adult'),
    '#default_value' => $total_lep_adult,
  );
  
  $form['form']['totallep'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Lep'),
    '#default_value' => $total_lep,
  );
  
  $form['form']['calpremale'] = array(
  '#type' => 'textfield',
    '#title' => t('Cal Pre-Adult Male'),
    '#default_value' => $cal_pre_male,
  );
  
  $form['form']['calprefemale'] = array(
  '#type' => 'textfield',
    '#title' => t('Cal Pre-Adult Female'),
    '#default_value' => $cal_pre_female,
  );
  
  $form['form']['calpreunid'] = array(
  '#type' => 'textfield',
    '#title' => t('Cal Pre-Adult Unidentified'),
    '#default_value' => $cal_pre_unid,
  );
  
  $form['form']['totalcalpre'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Cal Pre-Adult'),
    '#default_value' => $total_cal_pre,
  );
  
  $form['form']['caladultmale'] = array(
  '#type' => 'textfield',
    '#title' => t('Cal Adult Male'),
    '#default_value' => $cal_adult_male,
  );
  
  $form['form']['caladultfemale'] = array(
  '#type' => 'textfield',
    '#title' => t('Cal Adult Female'),
    '#default_value' => $cal_adult_female,
  );
  
  $form['form']['calgravid'] = array(
  '#type' => 'textfield',
    '#title' => t('Cal Gravid'),
    '#default_value' => $cal_gravid,
  );
  
  $form['form']['caladultunid'] = array(
  '#type' => 'textfield',
    '#title' => t('Cal Adult Unidentified'),
    '#default_value' => $cal_adult_unid,
  );
  
  $form['form']['totalcaladult'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Cal Adult'),
    '#default_value' => $total_cal_adult,
  );
  
  $form['form']['totalcal'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Cal'),
    '#default_value' => $total_cal,
  );
  
  $form['form']['leppremale'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Male'),
    '#default_value' => $lep_pre_male,
  );
  
  $form['form']['lepprefemale'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Female'),
    '#default_value' => $lep_pre_female,
  );
  
  $form['form']['leppreunid'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Pre-Adult Unidentified'),
    '#default_value' => $lep_pre_unid,
  );
  
  $form['form']['totalleppre'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Lep Pre-Adult'),
    '#default_value' => $total_lep_pre,
  );
  
  $form['form']['lepadultmale'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Adult Male'),
    '#default_value' => $lep_adult_male,
  );
  
  $form['form']['lepadultfemale'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Adult Female'),
    '#default_value' => $lep_adult_female,
  );
  
  $form['form']['lepgravid'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Gravid'),
    '#default_value' => $lep_gravid,
  );
  
  $form['form']['lepadultunid'] = array(
  '#type' => 'textfield',
    '#title' => t('Lep Adult Unidentified'),
    '#default_value' => $lep_adult_unid,
  );
  
  $form['form']['totallepadult'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Lep Adult'),
    '#default_value' => $total_lep_adult,
  );
  
  $form['form']['totallep'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Lep'),
    '#default_value' => $total_lep,
  );
  
  $form['form']['totalmotile'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Motile'),
    '#default_value' => $total_motile,
  );
  
  $form['form']['totallice'] = array(
  '#type' => 'textfield',
    '#title' => t('Total Lice'),
    '#default_value' => $total_lice,
  );
  
  $form['form']['scarchal'] = array(
  '#type' => 'textfield',
    '#title' => t('Scar - Chalimus'),
    '#default_value' => $scar_chal,
  );
  
  $form['form']['scarmob'] = array(
  '#type' => 'textfield',
    '#title' => t('Scar - Mobile'),
    '#default_value' => $scar_mob,
  );
  
  $form['form']['predmark'] = array(
  '#type' => 'textfield',
    '#title' => t('Predatory Marks'),
    '#default_value' => $pred_mark,
  );
  
  $form['form']['hem'] = array(
  '#type' => 'textfield',
    '#title' => t('Bleeding'),
    '#default_value' => $hem,
  );
  
  $form['form']['mateguard'] = array(
  '#type' => 'textfield',
    '#title' => t('Mate Guard'),
    '#default_value' => $mate_guard,
  );
  
  $form['form']['pinbelly'] = array(
  '#type' => 'textfield',
    '#title' => t('Pinch Belly'),
    '#default_value' => $pin_belly,
  );
  
  $form['form']['tissuedamage'] = array(
  '#type' => 'textfield',
    '#title' => t('Tissue Damage'),
    '#default_value' => $tissue_damage,
  );
  
  $form['form']['comments'] = array(
  '#type' => 'textfield',
    '#title' => t('Comments'),
    '#default_value' => $comments,
  );
//  var_dump($entries);

  return $form;
}

function bampeditfield_menu_callback($id=NULL) {
  return drupal_get_form('bampeditfield_form', $id);
}