<?php

class Santo {
  private $pid =NULL;
  private $item=NULL;
  private $utilityClass = NULL;
  private $dataStreams_list=NULL;
  function Santo($pid) {
   $this->pid = $pid;
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    module_load_include('inc', 'fedora_repository', 'api/dublin_core');
    $this->item = new Fedora_Item($pid);
    $this->datastreams_list =  $this->item->get_datastreams_list_as_SimpleXML();
    drupal_add_css(drupal_get_path('module', 'fedora_repository') .'/stylesheets/coccc.css');
  }

  function getModsAsHtml($modsString) {
    $modsXml = new SimpleXMLElement($modsString);
    $modsXml->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3");
    $title = $modsXml->xpath('/mods:mods/mods:titleInfo/mods:title');
    $author = $modsXml->xpath('//mods:role[mods:roleTerm = "creator"]/../../mods:name/mods:namePart');
    $typeOfResource = $modsXml->xpath('//mods:typeOfResource');
    //$dateCreated = $modsXml->xpath('//mods:dateCreated[@keyDate="yes"]');
    //$dateDigitized = $modsXml->xpath('//mods:dateCaptured');
    $rightsStatement = $modsXml->xpath('//mods:accessCondition[@type="useAndReproduction"]');
    $description = $modsXml->xpath('//mods:abstract');
    $noteAdmin = $modsXml->xpath('//mods:note[@type="admin"]');
    $subjectTopic= $modsXml->xpath('//mods:subject/mods:topic');
    $subjectCorporate = $modsXml->xpath('//mods:subject/mods:name[@type="corporate"]/mods:namePart');
    $subjectPersonal = $modsXml->xpath('//mods:subject/mods:name[@type="personal"]/mods:namePart');
    $subjectTime = $modsXml->xpath('//mods:subject/mods:temporal');
    $subjectGeo = $modsXml->xpath('//mods:subject/mods:geographic');
    $subjectForm = $modsXml->xpath('//mods:subject/mods:genre');

    $conservationHistory = $modsXml->xpath('//mods:note[@type="conservation_history"]');
    $aquisition = $modsXml->xpath('//mods:note[@type="aquisition"]');
    $exhibitonHistory = $modsXml->xpath('//mods:note[@type="exhibition_history"]');
    $noteGeneral = $modsXml->xpath('//mods:note[@type="general"]');
    
    $placeOfPublication = $modsXml->xpath('//mods:originInfo/mods:place/mods:placeTerm');
    $publisher = $modsXml->xpath('//mods:originInfo/mods:publisher');
    $language = $modsXml->xpath('//mods:recordInfo/mods:languageOfCataloging/mods:languageTerm[@authority="iso639-2"]');
    $series = $modsXml->xpath('//mods:relatedItem/mods:titleInfo[@type="series"]/mods:title');

    $html .= '<table align="left"><tr><td valign="top" align="right" class="fedora_mods_field" width="20%">';
    $html .='<strong>Title</strong></td><td>';
    foreach($title as $t) {
      $html .= $t.'<br />';
    }
    $html .= $this->htmlSnippet("Author",$author);
    $html .= $this->htmlSnippet("Type of Resource",$typeOfResource);
    $html .= $this->htmlSnippet("Digital Origin",$digitalOrigin);
    $html .= $this->htmlSnippet("Date Created",$dateCreated);
    $html .= $this->htmlSnippet("Date Digitized",$dateDigitized);
    $html .= $this->htmlSnippet("Rights Statement",$rightsStatement);
    $html .= $this->htmlSnippet("Description",$description);
    $html .= $this->htmlSnippet("Note",$noteAdmin);
    $html .= $this->htmlSnippet("Topic",$subjectTopic);
    $html .= $this->htmlSnippet("Corporate Name(s)",$subjectCorporate);
    $html .= $this->htmlSnippet("Subject (Personal)",$subjectPersonal);
    $html .= $this->htmlSnippet("Subject (Time)",$subjectTime);
    $html .= $this->htmlSnippet("Geography",$subjectGeo);
    $html .= $this->htmlSnippet("Form",$subjectForm);

   $html .= $this->htmlSnippet("Conservation History", $conservationHistory);
    $html .= $this->htmlSnippet("Aquisition", $aquisition);
    $html .= $this->htmlSnippet("Exhibition History", $exhibitonHistory);
    $html .= $this->htmlSnippet("Note General", $noteGeneral );

    $html .= $this->htmlSnippet("Place of Publication",$placeOfPublication);
    $html .= $this->htmlSnippet("Publisher",$publisher);
    $html .= $this->htmlSnippet("Language",$language);
    $html .= $this->htmlSnippet("Series",$series);
    $html .='</td></tr></table></div></div><br />';
    $html .= $this->parse_related_items($modsXml);
    return $html;
  }

  function parse_related_items($modsXml){
      $modsRelatedItems = $modsXml->xpath('//mods:relatedItem');
     
      $html =  '<table border = "0" align="left"><tr><td valign="top" align="right" class="fedora_mods_field" width="25%">';
      foreach($modsRelatedItems as $relatedItem){
          $identifier = $relatedItem->xpath('mods:identifier');
          if(!empty($identifier)){
          //$html .= $this->htmlSnippet('Identifier',$identifier);
              $html .= '</td></tr><tr><td valign="top" align="right" class="mods_field"><strong>Identifier</strong></td><td>';
              $html .= '<a href = "'.$base_url.'/fedora/repository/'.$this->pid.'/'.(string)$identifier[0] . '.jpg/'. drupal_urlencode((string)$identifier[0]).'.jpg">'.(string)$identifier[0].'</a><br />';
          }
          $dateCreated = $relatedItem->xpath('mods:originInfo/mods:dateCreated');
          if(!empty($dateCreated)){
              //$type = $relatedItem->xpath('@type');
              $html .= $this->htmlSnippet('Date Created: ',$dateCreated);
          }

          $title = $relatedItem->xpath('mods:titleInfo[@displayLabel!="Collection"]/mods:title');
          if(!empty($title)){
              //$label = $relatedItem->xpath('mods:titleInfo/@displayLabel');
              $html .= $this->htmlSnippet(' Title: ', $title);
          }
           $digitalOrigin = $relatedItem->xpath('mods:physicalDescription/mods:digitalOrigin');
          if(!empty($digitalOrigin[0])){
              //$label = $relatedItem->xpath('mods:titleInfo/@displayLabel');
              $html .= $this->htmlSnippet('Digital Origin: ', $digitalOrigin);
          }
      }
      $html .='</td></tr></table></div></div><br />';
      return $html;
     
  }

  function htmlSnippet($fieldName, $value) {
    $html = '</td></tr>';
    $html .='<tr><td valign="top" align="right" class="mods_field"><strong>'.$fieldName.'</strong></td><td>';
    if(!empty($value)){
    //if($fieldName == 'Author' || $fieldName == 'Topic' || $fieldName == "Place of Publication" || $fieldName == "Publisher") {
    //    foreach($value as $va){
    //    $html .= '<a href = "'.$base_url.'/islandora/solr/search/'.$fieldName.drupal_urlencode(':"'.$va.'"').'">'.$va .'</a><br />';
    //    }
    //}   else {

    foreach( $value as $va) {
      $html .= $va .'<br />';
    }
   // }
    return $html;
    } else {
      return '';
    }

  }


  //finds datastreams that have a certain mimetype
  //excludes ones that contain any strings in the array $exclude
  //finds datastreams that have a certain mimetype
  //excludes ones that contain any strings in the array $exclude
  function getListOfStreams($mimeTypeToRetrieve, $exclude=null) {
    // $list_of_streams = array();
    global $base_url;
    $html ='';

    if ($this->datastreams_list != FALSE) {
      foreach ($this->datastreams_list->datastreamDef as $ds) {
        if($mimeTypeToRetrieve == $ds->MIMEType ) {
          // $list_of_streams[] = $ds;
          $include = TRUE;
          if(isset($exclude)) {
            foreach($exclude as $value) {
              if(stristr($ds->ID, $value)) {
                $include = false;
              }
            }
          }
          if($include) {
            $label = $ds->label;
            if(empty($label)){
              $label = $ds->ID;
            }
            $html .= '<a href = "'.$base_url.'/fedora/repository/'.$this->pid.'/'.$ds->ID . '/'. drupal_urlencode($label).'">'.$label.'</a><br />';
          }
        }
      }
    }
    //return $list_of_streams;
    return $html;
  }

  //finds datastreams that have a certain mimetype
  //excludes ones that contain any strings in the array $exclude
  //returns an array of streams we are interested in
  function getListOfStreamsRaw($mimeTypeToRetrieve, $exclude=null) {
    $list_of_streams = array();
    global $base_url;
    $html ='';

    if ($this->datastreams_list != FALSE) {
      foreach ($this->datastreams_list->datastreamDef as $ds) {
        if($mimeTypeToRetrieve == $ds->MIMEType ) {
          // $list_of_streams[] = $ds;
          $include = TRUE;
          if(isset($exclude)) {
            foreach($exclude as $value) {
              if(stristr($ds->ID, $value)) {
                $include = false;
              }
            }
          }
          if($include) {
            $label = $ds->label;
            if(empty($label)){
              $label = $ds->ID;
            }
              $list_of_streams[] = $ds->ID.'/'.$label;
            }
        }
      }
    }

    return $list_of_streams;
  }


  function showTabs() {
    module_load_include('inc', 'fedora_repository', 'plugins/tagging_form');
    module_load_include('inc', 'fedora_repository', 'plugins/ShowStreamsInFieldSets');
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $ssif = new ShowStreamsInFieldSets($this->pid);
    global $base_url;

    $tabset = array();

    $tabset['my_tabset'] = array(
            '#type' => 'tabset',
    );

   // $listOfStreams = $this->getListOfStreams("audio/mpeg");

    global $user;
    $modsText = $this->item->get_datastream_dissemination("MODS");
    $modsAsHtml = $this->getModsAsHtml($modsText);
    $modsAsHtml = '<div class="fedora_object_container"><div class="fedora_mods_container">'.$modsAsHtml;
     $html ='</div><div class="fedora_links_container">';
    //$html .='Audio <br />'.$this->getListOfStreams('audio/mpeg');
    //$htmlList = $this->getListOfStreams('image/jpeg',array('TN','thumbnail','preview','web'));
    if(!empty($htmlList)){
         $html .='Images <br />'.$htmlList.'</div>';
    }

    $html .='</div><br />';
    $modsAsHtml .=$html;
    $tabset['my_tabset']['second_tab'] = array(
            '#type' => 'tabpage',
            '#title' => t('Descripton'),
            '#content' => $modsAsHtml,
    );


  	$rawImageList = $this->getListOfStreamsRaw('image/jpeg',array('TN','thumbnail','preview','web'));
  $tabset['my_tabset']['4'] = array(
            '#type' => 'tabpage',
            '#title' => t('Image Viewer'),
            '#content' => $ssif->showImageInFlashPlayer($rawImageList),
     );
    // Render the tabset.
    return '<div class="tabs">'.tabs_render($tabset).'</div>';
  }
}

?>