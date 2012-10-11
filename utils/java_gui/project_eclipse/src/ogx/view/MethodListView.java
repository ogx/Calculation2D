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
	JList list = null;
	final ImagesStruct images_struct;
	final MethodListModel methodlist;
	
	public MethodListView(MethodListModel methodlist, JComponent owner_container, ImagesStruct images_struct) {
		this.images_struct = images_struct;
		this.methodlist = methodlist;
		MethodModel method = null;
		ParamModel param = null;
		JPanel panel = null;
		empty_panel.setBorder(BorderFactory.createTitledBorder("Parameters"));
		((TitledBorder)empty_panel.getBorder()).setTitleJustification(TitledBorder.CENTER);
		container = owner_container;
		current_component = empty_panel;
		container.add(current_component);
		current_component.setPreferredSize(container.getSize());
		
		for (int i = 0; i < methodlist.getSize(); ++i) {
			method = (MethodModel)methodlist.getElementAt(i);
			panel = new JPanel();
			BoxLayout boxlayout = new BoxLayout(panel, BoxLayout.Y_AXIS);
			panel.setLayout(boxlayout);
			panel.setBorder(BorderFactory.createTitledBorder("Parameters"));
		    ((TitledBorder)panel.getBorder()).setTitleJustification(TitledBorder.CENTER);
		    for (int j = 0; j < method.getParams().size(); ++j) {
		    	param = method.getParams().get(j);
		    	panel.add(new ParamPanel(param.getName(), param.getType(), param.getValue(), this));
		    }
		    method_panels.add(panel);
		}
		
	}
	
	public void bindList(JList list) {
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
		// TODO: this method is meant to query view and return selected method with params to the controller
		// TODO: test this method
		MethodModel result = null;
		int index = method_panels.indexOf(current_component);
		if (index >= 0) {
			result = (MethodModel) methodlist.getElementAt(index);
			for (int i = 0; i < current_component.getComponentCount(); ++i) {
				ParamPanel current_panel = (ParamPanel)current_component.getComponent(i);
				for (int j = 0; j < result.getParams().size(); ++j) {
					if (current_panel.getParamName().equals(result.getParams().get(j).getName())) {
						result.getParams().get(j).setValue(current_panel.getContent());
						break;
					}
				}
			}
			
			for (int i = 0; i < result.getParams().size(); ++i) {
				
			}
		}
		return result;
	}

	@Override
	public void valueChanged(ListSelectionEvent arg0) {
		
		container.remove(current_component);
		current_component = empty_panel;
//		if (arg0.getFirstIndex() == arg0.getLastIndex()) {
			if (!method_panels.isEmpty() && list != null) {
				current_component = method_panels.get(list.getSelectedIndex());
			}
//		}
		container.add(current_component);
		container.setPreferredSize(current_component.getPreferredSize());
		container.updateUI();
		container.updateUI();
	}

	@Override
	public void actionPerformed(ActionEvent arg0) {
		// TODO: test automatic layer names in combo
		// currently not working -> check this out
//		if (arg0.getSource().getClass().equals(JComboBox.class)) {
//			String image_name = ((JComboBox)arg0.getSource()).getSelectedItem().toString();
//			Vector<String> layer_names = images_struct.getLayerNames(image_name);
//			for (int i = 0; i < method_panels.size(); ++i) {
//				JPanel method_panel = method_panels.get(i);
//				for (int j = 0; j < method_panel.getComponentCount(); ++j) {
//					ParamPanel current_panel = (ParamPanel)method_panel.getComponent(j);
//					if (current_panel.getParamType().get() == ParamType.LAYERNAME) {
//						current_panel.setContent(layer_names, 0);
//					}
//				}
//			}
//		}
	}

}
