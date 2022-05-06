config_rtc_alpha:
	@rm -f xclbin_rtc_alpha.cfg
	@echo "[connectivity]"                                                            > xclbin_rtc_alpha.cfg
	@echo "stream_connect=rtc_gen_1.dataout_axis_m:alpha_mix_1.time_img_input"       >> xclbin_rtc_alpha.cfg
	@echo "stream_connect=alpha_mix_1.mix_img_output:strm_dump_1.data_input"         >> xclbin_rtc_alpha.cfg

config_rtc_gen_test:
	@rm -f xclbin_rtc_gen_test.cfg
	@echo "[connectivity]"                                                            > xclbin_rtc_gen_test.cfg
	@echo "stream_connect=rtc_gen_1.dataout_axis_m:strm_dump_1.data_input"           >> xclbin_rtc_gen_test.cfg

