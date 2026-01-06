/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

enum reg_num {
		Input_Port_0, Input_Port_1, Input_Port_2, reserved0,
		Output_Port_0, Output_Port_1, Output_Port_2, reserved1,
		Polarity_Inversion_port_0, Polarity_Inversion_port_1, Polarity_Inversion_port_2, reserved2,
		Configuration_port_0, Configuration_port_1, Configuration_port_2,
		Output_drive_strength_register_port_0A=0x40, Output_drive_strength_register_port_0B,
		Output_drive_strength_register_port_1A, Output_drive_strength_register_port_1B,
		Output_drive_strength_register_port_2A, Output_drive_strength_register_port_2B,
		reserved3, reserved4,
		Input_latch_register_port_0, Input_latch_register_port_1, Input_latch_register_port_2, reserved5,
		Pull_up_pull_down_enable_register_port_0, Pull_up_pull_down_enable_register_port_1, Pull_up_pull_down_enable_register_port_2, reserved6,
		Pull_up_pull_down_selection_register_port_0, Pull_up_pull_down_selection_register_port_1, Pull_up_pull_down_selection_register_port_2, reserved7,
		Interrupt_mask_register_port_0, Interrupt_mask_register_port_1, Interrupt_mask_register_port_2, reserved8,
		Interrupt_status_register_port_0, Interrupt_status_register_port_1, Interrupt_status_register_port_2, reserved9,
		Output_port_configuration_register, reserved10, reserved11, reserved12,
		Interrupt_edge_register_port_0A, Interrupt_edge_register_port_0B,
		Interrupt_edge_register_port_1A, Interrupt_edge_register_port_1B,
		Interrupt_edge_register_port_2A, Interrupt_edge_register_port_2B,
		reserved13, reserved14,
		Interrupt_clear_register_port_0, Interrupt_clear_register_port_1, Interrupt_clear_register_port_2, reserved15,
		Input_status_port_0, Input_status_port_1, Input_status_port_2, reserved16,
		Individual_pin_output_port_0_configuration_register, Individual_pin_output_port_1_configuration_register, Individual_pin_output_port_2_configuration_register, reserved17,
		Switch_debounce_enable_0, Switch_debounce_enable_1, Switch_debounce_count,
};
