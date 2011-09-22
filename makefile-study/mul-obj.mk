
bigoutput littleoutput : 
	@echo  ${VERSION}
	@echo  $(PWD)
	@echo  -$(subst output,,$@)

