
#pragma once

#if defined(BUILD_MONOLITHIC)

#ifdef __cplusplus
extern "C" {
#endif

	extern int tessdoc_example_basic_main(void);
	extern int tessdoc_example_get_component_images_main(void);
	extern int tessdoc_example_lstm_choices_main(void);
	extern int tessdoc_example_openCV_main(void);
	extern int tessdoc_example_result_iterator_main(void);
	extern int tessdoc_example_set_rectangle_main(void);
	extern int tessdoc_example_user_patterns_main(void);
	extern int tessdoc_example_osd_main(int argc, const char** argv);

	extern int tessdoc_example_choices_iterator_main(void);
	extern int tessdoc_example_C_api_main(int argc, const char **argv);
	extern int tessdoc_example_hocr_altsym_main(void);
	extern int tessdoc_example_monitoring_main(void);
	extern int tessdoc_example_monitoring_cpp20_main(void);
	extern int tessdoc_example_alt_osd_main(void);
	extern int tessdoc_example_searchable_pdf_main(void);

#ifdef __cplusplus
}
#endif

#endif
