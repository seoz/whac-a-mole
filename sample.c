#include <Elementary.h>

struct _app_data
{
   Evas_Object *win, *edje;
   Evas_Object *mole[9];
   Ecore_Timer *timer;
   int score, count;
};
typedef struct _app_data app_data;

static void _win_free_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   free(data);
}

static Eina_Bool
_timer_cb(void *data)
{
    int i, r;
    char buf[256];
    app_data *ad = (app_data *)data;

    r = rand()%9;
    elm_layout_signal_emit(ad->mole[r], "elm,state,show", "elm");
    ad->count++;

    if (ad->count >= 10)
    {
        ad->timer = NULL;
        return ECORE_CALLBACK_CANCEL;
    }

    return ECORE_CALLBACK_RENEW;
}

static void
_mole_clicked_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
    app_data *ad = (app_data *)data;
    char score[10];

    ad->score++;
    sprintf(score, "%d", ad->score);
    elm_object_part_text_set(ad->edje, "score.text", score);
}

static void
_start_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
    app_data *ad = (app_data *)data;

    if (!(ad->timer))
    {
        srand(time(NULL));
        ad->score = 0;
        ad->count = 0;
        elm_object_part_text_set(ad->edje, "score.text", "0");

        ad->timer = ecore_timer_add(1.0, _timer_cb, ad);
    }
}

int main(int argc, char **argv)
{
    int i;
    char buf[256];
    app_data *ad;
    Evas_Object *win, *edje, *mole[9];
    ad = calloc(1, sizeof(app_data));

    elm_init(argc, argv);

    elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

    win = elm_win_util_standard_add("catch-a-mole", "Catch a mole");
    ad->win = win;
    elm_win_autodel_set(win, EINA_TRUE);
    evas_object_resize(win, 400, 300);
    evas_object_move(win, 100, 100);
    evas_object_show(win);
    evas_object_event_callback_add(win, EVAS_CALLBACK_FREE, _win_free_cb, ad);

    edje = elm_layout_add(win);
    ad->edje = edje;
    elm_layout_file_set(edje, "sample.edj", "catch_a_mole");
    elm_layout_signal_callback_add(edje, "start", "", _start_cb, ad);
    evas_object_size_hint_weight_set(edje, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win, edje);
    evas_object_show(edje);

    for (i = 0; i < 9 ; i++)
    {
        mole[i] = elm_layout_add(edje);
        ad->mole[i] = mole[i];
        elm_layout_file_set(mole[i], "mole.edj", "mole");
        sprintf(buf, "swallow%d", i + 1);
        elm_object_part_content_set(edje, buf, mole[i]);
        elm_layout_signal_callback_add(mole[i], "clicked", "", _mole_clicked_cb, ad);
    }

    elm_run();

    elm_shutdown();

    return 0;
}


