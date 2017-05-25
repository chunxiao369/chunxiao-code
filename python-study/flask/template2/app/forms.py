from flask_wtf import FlaskForm as BaseForm
from wtforms import TextField,PasswordField
from wtforms.validators import Required,Length

class LoginForm(BaseForm):
    username= TextField('username', validators = [Required()])
    password = PasswordField('password', validators = [Required()])
