OUT_FORMAT	:= png
DOT_TOOL	:= dot
PIC_TOOL	:= eog
RUN_FLAGS	:=

ifdef f
	TARGET := $(f)
endif
TARGET		?= t1

$(TARGET): $(TARGET).dot
	$(DOT_TOOL) -T$(OUT_FORMAT) $^ -o $@.$(OUT_FORMAT)
	$(PIC_TOOL) $@.$(OUT_FORMAT) $(RUN_FLAGS)

clean:
	rm *.$(OUT_FORMAT)

