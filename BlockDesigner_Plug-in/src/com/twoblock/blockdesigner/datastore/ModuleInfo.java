package com.twoblock.blockdesigner.datastore;

import java.util.ArrayList;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;


public class ModuleInfo {
	private JSONObject ModuleObject;
	private JSONObject PortObject;
	private JSONObject ParameterObject;
	private JSONObject RegisterObject;

	private JSONArray PortInfoList;
	private JSONArray ParameterInfoList;
	private JSONArray RegisterInfoList;

	public ArrayList<Module> mList;

	public ModuleInfo() {
		PortObject = new JSONObject();
		ModuleObject = new JSONObject();
		ParameterObject = new JSONObject();
		RegisterObject = new JSONObject();
		mList = new ArrayList<Module>();
	}

	public ModuleInfo(JSONArray JSON_ModuleList) {

		mList = new ArrayList<Module>();
		
		if(JSON_ModuleList != null){
			
			int Module_List_Size = JSON_ModuleList.size();
			for (int i = 0; i < Module_List_Size; i++) {
				Module module = new Module();
				ModuleObject = (JSONObject) JSON_ModuleList.get(i);
				module.module_name = (String) ModuleObject.get("module_name");
				module.module_location = (String) ModuleObject.get("module_location");
				module.module_type = (String) ModuleObject.get("module_type");

				/*
				 * add port
				 */
				int SM_Port_Index=-1;
				try{
					PortInfoList = (JSONArray) ModuleObject.get("port");
					int Port_List_Size = PortInfoList.size();
					for (int j = 0; j < Port_List_Size; j++) {
						PortObject = (JSONObject) PortInfoList.get(j);
						String p_name = (String) PortObject.get("port_name");
						String p_sc_type = (String) PortObject.get("sc_type");
						String p_data_type = (String) PortObject.get("data_type");



						Port port = new Port();
						port.port_name = p_name;
						port.sc_type = p_sc_type;
						port.data_type = p_data_type;
						if(p_data_type.contains("SM"))
						{
							SM_Port_Index++;
							port.SM_Index= SM_Port_Index;
						}
						else 
							port.SM_Index= -1;

						port.startAddr="0";					
						port.addrSize="0";

						port.Parent = module;
						port.Dest_Port=null;
						port.cmb_dPort=null;

						module.Port_List.add(port);
					}
				}catch(Exception e){
				}

				/*
				 * add parameter
				 */
				try{
					ParameterInfoList = (JSONArray) ModuleObject.get("parameter");
					int Parameter_List_Size = ParameterInfoList.size();
					for (int j = 0; j < Parameter_List_Size; j++) {
						ParameterObject = (JSONObject) ParameterInfoList.get(j);
						String par_name = (String) ParameterObject.get("parameter_name");
						String par_data_type = (String) ParameterObject.get("type");
						String par_default_value = (String) ParameterObject.get("value");
						String par_bits_wide = (String) ParameterObject.get("bits_wide");

						Parameter par = new Parameter();
						par.par_name = par_name;
						par.data_type = par_data_type;
						par.default_value = par_default_value;
						par.bits_wide = par_bits_wide;

						module.Parameter_List.add(par);					
					}
				}catch(Exception e){
				}

				/*
				 * add register
				 */
				try{
					RegisterInfoList = (JSONArray) ModuleObject.get("register");
					int Register_List_Size = RegisterInfoList.size();
					for (int j = 0; j < Register_List_Size; j++) {
						RegisterObject = (JSONObject) RegisterInfoList.get(j);
						String reg_name = (String) RegisterObject.get("register_name");
						String reg_data_type = (String) RegisterObject.get("type");
						String reg_bits_wide = (String) RegisterObject.get("bits_wide");

						Register reg = new Register();
						reg.reg_name = reg_name;
						reg.data_type = reg_data_type;
						reg.bits_wide = reg_bits_wide;

						module.Register_List.add(reg);					
					}
				}catch(Exception e){
				}

				mList.add(module);
			}
		}
	}
}
