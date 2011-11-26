<?php
class DevelCardConfigModel extends Model{
	function DevelCardConfigModel(){
		$option = develcard_get_permission_option();
		$this->develcard_access = $this->radio(tt('DevelCard report permission'), $option);
		
		$this->develcard_pixel_per_point = $this->spin(tt('pixel per point for workload & karma report'), 1, 100);
		$this->develcard_load_color = $this->color('workload font color');
		$this->develcard_load_bg_color = $this->color('workload background color');
		$this->develcard_point_color = $this->color('karma font color');
		$this->develcard_point_bg_color = $this->color('karma background color');
		
		$this->develcard_comment_write_label  = $this->title(tt('comment label'));
		$this->develcard_comment_page_length = $this->spin(tt('comment number per page'), 5, 250);
		$this->develcard_list_per_page = $this->spin(tt('Number of DevelCard per page'), 5, 250);
		
		$this->develcard_hight_color = $this->color('high priority color');
		$this->develcard_medium_color = $this->color('medium priority color');
		$this->develcard_low_color = $this->color('low priority color');
		$this->develcard_no_effect_color = $this->color('no-effect priority color');
		
		$this->develcard_type = $this->grid(tt('DevelCard Type'));
		$cardType = cc('develcard_type');
		$cardTypeName= Model::char(tt('type name'));
		if(is_array($cardType)){
			$cardTypeName->data = $cardType;
		}
		$this->develcard_type->append('develcard_type', $cardTypeName);
	}
}
?>
