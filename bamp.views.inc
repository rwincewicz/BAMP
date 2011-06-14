<?php

function bamp_views_data() {
  $data = array();
  $data['bamp_results']['table'] = array(
    'group' => 'BAMP',
    'title' => 'BAMP',
    'help' => 'BAMP',
  );
  
  $data['bamp_results']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'id',
    ),
  );

  $data['bamp_results']['table']['base'] = array(
    'field' => 'id',
    'title' => 'ID',
    'help' => 'ID',
  );

  $data['bamp_results']['id'] = array(
    'title' => t('Result ID'),
    'help' => t('Result ID'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['pen'] = array(
    'title' => t('Pen no'),
    'help' => t('Pen number'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['fish_id'] = array(
    'title' => t('Fish ID'),
    'help' => t('Fish ID'),
    'field' => array(
     'handler' => 'views_handler_field', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['caligus'] = array(
    'title' => t('Caligus'),
    'help' => t('Number of caligus'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['chalimus'] = array(
    'title' => t('Chalimus'),
    'help' => t('Number of chalimus'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['female'] = array(
    'title' => t('Female'),
    'help' => t('Number of female'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['male'] = array(
    'title' => t('Male'),
    'help' => t('Number of male'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['gravid'] = array(
    'title' => t('Gravid'),
    'help' => t('Number of gravid'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['lab_id'] = array(
    'title' => t('Lab ID'),
    'help' => t('Lab ID'),
    'field' => array(
     'handler' => 'views_handler_field', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['method'] = array(
    'title' => t('Method ID'),
    'help' => t('Method ID'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['pre_adult'] = array(
    'title' => t('Pre adult'),
    'help' => t('Pre adult'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'field' => 'pre_adult',
      'base' => 'bamp_lab_results',
      'base field' => 'fish_id',
    ),
  );
  
  $data['bamp_results']['site_id'] = array(
    'title' => t('Site ID'),
    'help' => t('Site ID'),
    'field' => array(
     'handler' => 'views_handler_field', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['size'] = array(
    'title' => t('Size/g'),
    'help' => t('Size of fish in grams'),
    'field' => array(
     'handler' => 'views_handler_field_numeric', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['user'] = array(
    'title' => t('User'),
    'help' => t('Person who collected the samples'),
    'field' => array(
     'handler' => 'views_handler_field', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['date'] = array(
    'title' => t('Date'),
    'help' => t('Date of collection'),
    'field' => array(
     'handler' => 'views_handler_field', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['comments'] = array(
    'title' => t('Comments'),
    'help' => t('Comments'),
    'field' => array(
     'handler' => 'views_handler_field', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_results']['scarring'] = array(
    'title' => t('Scarring'),
    'help' => t('Scarring'),
    'field' => array(
     'handler' => 'views_handler_field', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['bamp_lab_results']['table'] = array(
    'group' => 'BAMP',
    'title' => 'BAMP lab results',
    'help' => 'BAMP lab results',
  );
  
  $data['bamp_lab_results']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'id',
    ),
  );

  $data['bamp_lab_results']['table']['base'] = array(
    'field' => 'id',
    'title' => 'ID',
    'help' => 'ID',
  );
  
  $data['bamp_lab_results']['species'] = array(
    'title' => t('Lab species'),
    'help' => t('Species as identified in the lab'),
    'field' => array(
     'handler' => 'views_handler_field', 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'field' => 'fid',
      'base' => 'bamp_results',
      'base field' => 'fid',
      'label' => 'Lab species',
    ),
  );
  
  return $data;  
}
