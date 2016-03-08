/* GStreamer rtmp element unit test
 *
 * Copyright (C) 2010 Anthony Violo  <anthony.violo@ubicast.eu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <unistd.h>
 
#include <gst/check/gstcheck.h>

GST_START_TEST (test_still_image)
{
  GstElement *pipeline, *video_src, *video_enc, *muxer, *sink, *audio_src, *audio_enc;

  pipeline = gst_pipeline_new ("pipeline");

  video_src = gst_element_factory_make ("videotestsrc", NULL);
  gst_util_set_object_arg (G_OBJECT (video_src), "num-buffers", "100");
  gst_util_set_object_arg (G_OBJECT (video_src), "is-live", "true");

  video_enc = gst_element_factory_make ("x264enc", NULL);
  muxer = gst_element_factory_make ("flvmux", NULL);

  sink = gst_element_factory_make ("rtmpsink", NULL);
  gst_util_set_object_arg (G_OBJECT (sink), "location", "rtmp://yoururl.fr/vod/_definst_/stream/yourstream");
  gst_util_set_object_arg (G_OBJECT (sink), "sync", "true");
  gst_util_set_object_arg (G_OBJECT (sink), "reconnection-delay", "0");
  gst_util_set_object_arg (G_OBJECT (sink), "log-level", "4");
  gst_util_set_object_arg (G_OBJECT (sink), "flashver", "test");

  audio_src = gst_element_factory_make ("audiotestsrc", NULL);
  audio_enc = gst_element_factory_make ("faac", NULL);

  fail_unless ( gst_element_link_many (audio_src, audio_enc, muxer, NULL));
  fail_unless (gst_element_link_many (video_src, video_enc, muxer, sink, NULL));

  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  fail_unless_equals_int (gst_element_set_state (pipeline, GST_STATE_NULL),
      GST_STATE_CHANGE_SUCCESS);

  gst_object_unref (pipeline);
}

GST_END_TEST;

static Suite *
rtmp_suite (void)
{
  Suite *s = suite_create ("rtmp");
  TCase *tc_chain = tcase_create ("general");

  suite_add_tcase (s, tc_chain);
  tcase_add_test (tc_chain, test_still_image);

  return s;
}

int
main (int argc, char **argv)
{
  int nf;

  Suite *s = rtmp_suite ();
  SRunner *sr = srunner_create (s);

  gst_check_init (&argc, &argv);

  srunner_run_all (sr, CK_NORMAL);
  nf = srunner_ntests_failed (sr);
  srunner_free (sr);

  return nf;
}
