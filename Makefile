.PHONY: clean All

All:
	@echo "----------Building project:[ SimiasPSX - Debug ]----------"
	@"$(MAKE)" -f  "SimiasPSX.mk"
clean:
	@echo "----------Cleaning project:[ SimiasPSX - Debug ]----------"
	@"$(MAKE)" -f  "SimiasPSX.mk" clean
