#include <e.h>
#include "e_mod_main.h"

/* The typedef for this structure is declared inside the E code in order to
 * allow everybody to use this type, you dont need to declare the typedef, 
 * just use the E_Config_Dialog_Data for your data structures declarations */
struct _E_Config_Dialog_Data 
{
   int view_enable, notify; 
   double delay, pict_quality;
   char *viewer;
   char *path;
   
};

/* Local Function Prototypes */
static void *_create_data(E_Config_Dialog *cfd);
static void _free_data(E_Config_Dialog *cfd, E_Config_Dialog_Data *cfdata);
static void _fill_data(E_Config_Dialog_Data *cfdata);
static Evas_Object *_basic_create(E_Config_Dialog *cfd, Evas *evas, E_Config_Dialog_Data *cfdata);
static int _basic_apply(E_Config_Dialog *cfd, E_Config_Dialog_Data *cfdata);

/* External Functions */

/* Function for calling our personal dialog menu */
E_Config_Dialog *
e_int_config_shot_module(E_Container *con, const char *params __UNUSED__) 
{
   E_Config_Dialog *cfd = NULL;
   E_Config_Dialog_View *v = NULL;
   char buf[4096];

   /* is this config dialog already visible ? */
   if (e_config_dialog_find("takescreenshot", "advanced/takescreenshot")) return NULL;

   v = E_NEW(E_Config_Dialog_View, 1);
   if (!v) return NULL;

   v->create_cfdata = _create_data;
   v->free_cfdata = _free_data;
   v->basic.create_widgets = _basic_create;
   v->basic.apply_cfdata = _basic_apply;

   /* Icon in the theme */
   snprintf(buf, sizeof(buf), "%s/e-module-shot.edj", shot_conf->module->dir);

   /* create our config dialog */
   cfd = e_config_dialog_new(con, D_("Take Screenshot module"), "takescreenshot", 
                             "advanced/takescreenshot", buf, 0, v, NULL);

   e_dialog_resizable_set(cfd->dia, 1);
   shot_conf->cfd = cfd;
   return cfd;
}

/* Local Functions */
static void *
_create_data(E_Config_Dialog *cfd __UNUSED__) 
{
   E_Config_Dialog_Data *cfdata = NULL;

   cfdata = E_NEW(E_Config_Dialog_Data, 1);
   _fill_data(cfdata);
   return cfdata;
}

static void 
_free_data(E_Config_Dialog *cfd __UNUSED__, E_Config_Dialog_Data *cfdata) 
{
   shot_conf->cfd = NULL;
   E_FREE(cfdata);
}

static void 
_fill_data(E_Config_Dialog_Data *cfdata) 
{
   /* load a temp copy of the config variables */

   cfdata->view_enable = shot_conf->view_enable;
   cfdata->viewer = strdup(shot_conf->viewer);
   cfdata->path = strdup(shot_conf->path);
   cfdata->delay = shot_conf->delay;
   cfdata->pict_quality = shot_conf->pict_quality;
   cfdata->notify = shot_conf->notify;

}

static Evas_Object *
_basic_create(E_Config_Dialog *cfd __UNUSED__, Evas *evas, E_Config_Dialog_Data *cfdata) 
{
   Evas_Object *o = NULL, *of = NULL, *ow = NULL;

   o = e_widget_list_add(evas, 0, 0);

   of = e_widget_framelist_add(evas, D_("Settings"), 0);
   
   ow = e_widget_check_add(evas, D_(" Show notifications"), &(cfdata->notify));
   e_widget_framelist_object_append(of, ow);
   
   ow = e_widget_check_add(evas, D_(" Launch app after screenshot taking"), &(cfdata->view_enable));
   e_widget_framelist_object_append(of, ow);
      
   ow = e_widget_label_add(evas, D_("Application for opening:"));
   e_widget_framelist_object_append(of, ow);
   ow = e_widget_entry_add(evas, &cfdata->viewer, NULL, NULL, NULL);
   e_widget_size_min_set(ow, 100, 28);
   e_widget_framelist_object_append(of, ow);
   
   ow = e_widget_label_add(evas, D_("Folder for saving screenshots:"));
   e_widget_framelist_object_append(of, ow);
   ow = e_widget_entry_add(evas, &cfdata->path, NULL, NULL, NULL);
   e_widget_size_min_set(ow, 100, 28);
   e_widget_framelist_object_append(of, ow);
   
   ow = e_widget_label_add(evas, D_("Delay time [s]"));
   e_widget_framelist_object_append(of, ow);
   ow = e_widget_slider_add(evas, 1, 0, "%1.0f", 0.0, 10.0, 1.0, 0, &(cfdata->delay), NULL, 40);
   e_widget_framelist_object_append(of, ow);
   
   ow = e_widget_label_add(evas, D_("Image quality [1-99 = jpg, 100 = png]"));
   e_widget_framelist_object_append(of, ow);
   ow = e_widget_slider_add(evas, 1, 0, "%1.0f", 1.0, 100.0, 1.0, 0, &(cfdata->pict_quality), NULL, 40);
   e_widget_framelist_object_append(of, ow);
   
   e_widget_list_object_append(o, of, 1, 0, 0.5);

   of = e_widget_framelist_add(evas, D_("Info"), 0);   
   ow = e_widget_label_add(evas, D_("Bind Take Shot in Wndows:Action"));
   e_widget_framelist_object_append(of, ow);
   ow = e_widget_label_add(evas, D_("for window screenshot!"));
   e_widget_framelist_object_append(of, ow);
   
   e_widget_list_object_append(o, of, 1, 0, 0.5);

   return o;
}

static int 
_basic_apply(E_Config_Dialog *cfd __UNUSED__, E_Config_Dialog_Data *cfdata) 
{
  
   shot_conf->viewer = cfdata->viewer;
   shot_conf->notify = cfdata->notify;
   shot_conf->path = cfdata->path;
   shot_conf->view_enable = cfdata->view_enable;
   shot_conf->delay = cfdata->delay;
   shot_conf->pict_quality = cfdata->pict_quality;
   e_config_save_queue();
   
   return 1;
}