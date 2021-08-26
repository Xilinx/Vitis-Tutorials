config_rtc_alpha:
	@rm -f xclbin_rtc_alpha.cfg
	@echo "[connectivity]"                                                            > xclbin_rtc_alpha.cfg
	@echo "stream_connect=rtc_gen_1.dataout_axis_m:alpha_mix_1.time_img_input"       >> xclbin_rtc_alpha.cfg
	@echo "stream_connect=alpha_mix_1.mix_img_output:strm_dump_1.data_input"         >> xclbin_rtc_alpha.cfg
ifeq ($(PLATFORM), xilinx_u200_gen3x16_xdma_1_1_202020_1)
	@echo "slr=strm_dump_1:SLR0"                                                     >> xclbin_rtc_alpha.cfg
	@echo "slr=rtc_gen_1:SLR0"                                                       >> xclbin_rtc_alpha.cfg
	@echo "slr=alpha_mix_1:SLR0"                                                     >> xclbin_rtc_alpha.cfg
	@echo "[vivado]"                                                                 >> xclbin_rtc_alpha.cfg
	@echo "prop=run.impl_1.STEPS.OPT_DESIGN.IS_ENABLED=true"                         >> xclbin_rtc_alpha.cfg
	@echo "prop=run.impl_1.STEPS.PLACE_DESIGN.ARGS.DIRECTIVE=AltSpreadLogic_low"     >> xclbin_rtc_alpha.cfg
	@echo "prop=run.impl_1.STEPS.PHYS_OPT_DESIGN.IS_ENABLED=true"                    >> xclbin_rtc_alpha.cfg
	@echo "prop=run.impl_1.STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE=AggressiveExplore"   >> xclbin_rtc_alpha.cfg
	@echo "prop=run.impl_1.STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE=AlternateCLBRouting"    >> xclbin_rtc_alpha.cfg
endif

config_rtc_gen_test:
	@rm -f xclbin_rtc_gen_test.cfg
	@echo "[connectivity]"                                                            > xclbin_rtc_gen_test.cfg
	@echo "stream_connect=rtc_gen_1.dataout_axis_m:strm_dump_1.data_input"           >> xclbin_rtc_gen_test.cfg

