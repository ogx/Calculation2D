package ogx.view;

import java.awt.Component;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import ogx.model.ParamModel.ParamType;

public class ParamPanel extends JPanel {

	private static final long serialVersionUID = 1L;
	final ParamType type;
	final String caption;
	JComponent comp = null;
	Vector<String> content = new Vector<String>();

	public ParamPanel(String caption, ParamType type, String value, ActionListener combo_selection) {
		this.type = type;
		this.caption = caption;
		BoxLayout boxlayout = new BoxLayout(this, BoxLayout.Y_AXIS);
		this.setLayout(boxlayout);
		this.setAlignmentX(Component.CENTER_ALIGNMENT);
		this.setBorder(BorderFactory.createEmptyBorder(3,3,3,3));
		JLabel captionlabel = new JLabel();
		captionlabel.setAlignmentX(Component.LEFT_ALIGNMENT);
		captionlabel.setText(caption + " [" + this.type.toString() + "]");
		
		switch (type.get()) {
		case ParamType.STRING :
		case ParamType.INT :
		case ParamType.DOUBLE :
			//captionlabel.setText(caption + " [" + this.type.toString() + "]");
			this.add(captionlabel);
			comp = new JTextField();
			comp.setAlignmentX(Component.LEFT_ALIGNMENT);
			((JTextField)comp).setText(value);
			this.add(comp);
			break;
		case ParamType.BOOL:
			comp = new JCheckBox();
			comp.setAlignmentX(Component.LEFT_ALIGNMENT);
			((JCheckBox)comp).setText(caption);
			if (value.equals("yes")) ((JCheckBox)comp).setSelected(true);
			else ((JCheckBox)comp).setSelected(false);
			this.add(comp);
			break;
		case ParamType.IMAGENAME:
			//captionlabel.setText("Image name");
			this.add(captionlabel);
			comp = new JComboBox();
			comp.setAlignmentX(Component.LEFT_ALIGNMENT);
			((JComboBox)comp).addActionListener(combo_selection);
			this.add(comp);
			break;
		case ParamType.LAYERNAME:
			//captionlabel.setText("Layer name");
			this.add(captionlabel);
			comp = new JComboBox();
			comp.setAlignmentX(Component.LEFT_ALIGNMENT);
			this.add(comp);
			break;
		}
		this.add(Box.createVerticalStrut(5));
	}
	
	public void setContent(Vector<String> content, int selected_index) {
		this.content = content;
		if (type.get() == ParamType.IMAGENAME || type.get() == ParamType.LAYERNAME) {
			((JComboBox)comp).removeAllItems();
			for (int i = 0; i < this.content.size(); ++i) {
				((JComboBox)comp).addItem(this.content.get(i));
			}
			if (content.size() > 0) ((JComboBox)comp).setSelectedIndex(selected_index);
		}
	}
	
	public String getContent() {
		String result = "";
		switch (type.get()) {
		case ParamType.STRING :
		case ParamType.INT :
		case ParamType.DOUBLE :
			result = ((JTextField)comp).getText();
			break;
		case ParamType.BOOL :
			if (((JCheckBox)comp).isSelected()) result = "yes";
			else result = "no";
			break;
		case ParamType.IMAGENAME:
		case ParamType.LAYERNAME:
			result = ((JComboBox)comp).getSelectedItem().toString();
			break;
		}
		
		return result;
	}
	
	public ParamType getParamType() {
		return type;
	}
	
	public String getParamName() {
		return caption;
	}

}
