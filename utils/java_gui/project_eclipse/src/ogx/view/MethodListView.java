package ogx.view;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.border.TitledBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import ogx.model.ImagesStruct;
import ogx.model.MethodListModel;
import ogx.model.MethodModel;
import ogx.model.ParamModel;
import ogx.model.ParamModel.ParamType;

public class MethodListView implements
		ListSelectionListener, ActionListener {
	
	Vector<JPanel> method_panels = new Vector<JPanel>();
	JPanel empty_panel = new JPanel();
	JPanel current_component = null;
	JComponent container;
	JList<MethodModel> list = null;
	final ImagesStruct images_struct;
	MethodListModel methodlist;
	
	public MethodListView(MethodListModel methodlist, JComponent owner_container, ImagesStruct images_struct) {
		this.images_struct = images_struct;
		this.methodlist = methodlist;
		empty_panel.setBorder(BorderFactory.createTitledBorder("Parameters"));
		((TitledBorder)empty_panel.getBorder()).setTitleJustification(TitledBorder.CENTER);
		container = owner_container;
		current_component = empty_panel;
		container.add(current_component);
		current_component.setPreferredSize(container.getSize());
		
		for (int i = 0; i < methodlist.getSize(); ++i) {
			addMethodModelView((MethodModel)methodlist.getElementAt(i));
		}
		
	}
	
	public void addMethodModelView(MethodModel method_model) {
		JPanel panel = new JPanel();
		BoxLayout boxlayout = new BoxLayout(panel, BoxLayout.Y_AXIS);
		panel.setLayout(boxlayout);
		panel.setBorder(BorderFactory.createTitledBorder("Parameters"));
	    ((TitledBorder)panel.getBorder()).setTitleJustification(TitledBorder.CENTER);
	    for (int j = 0; j < method_model.getParams().size(); ++j) {
	    	ParamModel param = method_model.getParams().get(j);
	    	panel.add(new ParamPanel(param.getName(), param.getType(), param.getValue(), this));
	    }
	    method_panels.add(panel);
	}
	
	public void removeMethodModelView(int index) {
		//if (index >= 0 && index < method_panels.size()) {
			method_panels.remove(index);
			valueChanged(null);
		//}
	}
	
	public void clearMethodModelView() {
		method_panels.removeAllElements();
		valueChanged(null);
	}
	
	public void bindList(JList<MethodModel> list) {
		this.list = list;
	}
	
	public void update() {
		Vector<String> image_names = images_struct.getImageNames();
		for (int i = 0; i < method_panels.size(); ++i) {
			JPanel method_panel = method_panels.get(i);
			for (int j = 0; j < method_panel.getComponentCount(); ++j) {
				ParamPanel current_panel = (ParamPanel)method_panel.getComponent(j);
				if (current_panel.getParamType().get() == ParamType.IMAGENAME) {
					current_panel.setContent(image_names, 0);
				}
			}
		}
	}
	
	public MethodModel getCurrentMethodModel() {
		// TODO: test this method
		MethodModel result = null;
		int index = method_panels.indexOf(current_component);
		if (index >= 0) {
			updateMethodModel(index);
			result = (MethodModel) methodlist.getElementAt(index);
			
//			for (int i = 0; i < current_component.getComponentCount(); ++i) {
//				ParamPanel current_panel = (ParamPanel)current_component.getComponent(i);
//				for (int j = 0; j < result.getParams().size(); ++j) {
//					if (current_panel.getParamName().equals(result.getParams().get(j).getName())) {
//						result.getParams().get(j).setValue(current_panel.getContent());
//						break;
//					}
//				}
//			}
		}
		return result;
	}
	
	public void updateMethodModel(int index) {
		if (index >= 0 && index < method_panels.size()) {
			JPanel method_view = method_panels.get(index);
			MethodModel method_model = (MethodModel) methodlist.getElementAt(index);
			for (int i = 0; i < method_view.getComponentCount(); ++i) {
				ParamPanel current_panel = (ParamPanel) method_view.getComponent(i);
				for (int j = 0; j < method_model.getParams().size(); ++j) {
					if (current_panel.getParamName().equals(method_model.getParams().get(j).getName())) {
						method_model.getParams().get(j).setValue(current_panel.getContent());
						break;
					}
				}
			}
		}
	}

	@Override
	public void valueChanged(ListSelectionEvent arg0) {
		
		container.remove(current_component);
		current_component = empty_panel;
		if (!method_panels.isEmpty() && list != null) {
			int index = list.getSelectedIndex();
			if (index >= 0 && index < method_panels.size()) {
				current_component = method_panels.get(index);
			}
		}
		container.add(current_component);
		container.setPreferredSize(current_component.getPreferredSize());
		container.updateUI();
	}

	@Override
	public void actionPerformed(ActionEvent arg0) {	
		if (arg0.getSource().getClass().equals(JComboBox.class)) {
			Object selected_item = ((JComboBox)arg0.getSource()).getSelectedItem();
			if (selected_item != null) {
				String image_name = selected_item.toString();
				Vector<String> layer_names = images_struct.getLayerNames(image_name);
				Vector<String> existing_layers = new Vector<String>();
				for (int i = 0; i < method_panels.size(); ++i) {
					JPanel method_panel = method_panels.get(i);
					for (int j = 0; j < method_panel.getComponentCount(); ++j) {
						ParamPanel current_panel = (ParamPanel)method_panel.getComponent(j);
						if (current_panel.getParamType().get() == ParamType.LAYERNAME) {
							for (int k = 0; k < layer_names.size(); ++k) {
								if (layer_names.get(k).equals("R") || layer_names.get(k).equals("G") || layer_names.get(k).equals("B")) {  
									continue;
								}
								existing_layers.add(layer_names.get(k));
							}
							current_panel.setContent(existing_layers, 0);
						}
					}
				}
			}
		}
	}

}
