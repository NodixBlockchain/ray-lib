/**
 * <pre>
 * (Pav32) Selective quoting
 * IP.Board v3.2.2
 * Last Updated: September 13, 2011
 * </pre>
 *
 * @author 		Konrad "Pavulon" Szproncel
 * @copyright	(c) 2011 Konrad "Pavulon" Szproncel
 * @link		http://forum.invisionize.pl
 * @version		1.1.1 (Revision 10101)
 */

var _squote = window.IPBoard;

_squote.prototype.squote = {
	init: function ()
	{
		Debug.write("Initializing postsquote.js");
		
		document.observe("dom:loaded", ipb.squote.startObserve);
	},
	
	startObserve: function (e)
	{
		$$('.squote').each(function (e) {
			Event.stopObserving(e);
			e.style.display = "";
			e.observe( 'click', ipb.squote.selectiveQuote );
		});
		
	},

	/* ------------------------------ */
	/**
	 * Parsing ISO 8601 date by Daniel http://dansnetwork.com/2008/11/01/javascript-iso8601rfc3339-date-parser/
	*/
	parseIsoDate: function(s){
		var re=new RegExp(/(\d\d\d\d)\D?(\d\d)\D?(\d\d)\D?(\d\d)\D?(\d\d\D?(\d\d\.?(\d*))?)(Z|[+-]\d\d?(:\d\d)?)?/);

		var a=re.exec(s).slice(1).map( function(x,i)
		{
			if (i==6 && x)
			{
				x=parseInt(x,10)/Math.pow(10,x.length)*1000;
			}

			return parseInt(x,10)||0;
		} );
		
		return new Date( Date.UTC(a[0],a[1]-1,a[2],a[3]-(a[7]||0),a[4],a[5],a[6]) );
	},

	selectiveQuote: function( e )
	{
		Event.stop( e );
		
		// looking for selection
		var sel;
		if (document.selection)
		{
			//IE suxxx
			sel = document.selection.createRange();
		} else if (window.getSelection)
		{
			sel = window.getSelection();
		}
		else if (document.getSelection)
		{
			sel = document.getSelection();
		}
		else
			return alert( ipb.lang['no_selection'] );
		
		if ( sel.isCollapsed ) { return alert( ipb.lang['no_selection'] ); }

		var post, name, timestamp, content;
		
		var parent = 0;
		
		// litle IE "fix"
		var parent1 = ( document.selection ) ? sel.parentElement() : sel.anchorNode.parentNode;
		var parent2 = ( document.selection ) ? sel.parentElement() : sel.focusNode.parentNode;
		
		// avoid infinite loop
		var num = 20;

		while ( ! parent && --num>0 )
		{
			// check 4 correct object at the begining
			if ( !parent1.id || (parent1.id && !parent1.id.match(/post_id_/ ) ) )
			{
				if ( parent1.parentNode )
				{
					parent1 = parent1.parentNode;
				}
				else
				{
					break;
				}
			}
			// and the end of selection
			if ( !parent2.id || (parent2.id && !parent2.id.match(/post_id_/ ) ) )
			{
				if ( parent2.parentNode )
				{
					parent2 = parent2.parentNode;
				}
				else
				{
					break;
				}
			}

			if ( parent1 == parent2 && parent1.id && parent1.id.match(/post_id_/ ) )
			{
				// we've got it =]
				parent = parent1;
			} else if ( parent1.id && parent1.id.match(/post_id_/ ) &&  parent2.id && parent2.id.match(/post_id_/ ) )
			{
				// starting post has another id than ending
				break;
			}
		}
		
		if ( parent )
		{
			post = parent.id.replace('post_id_', '');
			if ( ! isNaN(post) )
			{
				name = $('post_id_'+post).down('.url.fn').innerHTML;
				if ( name.match( '<' ) )
				{
					var re = new RegExp(/(?:<.*>)(.*)(?:<\/.*>)/);
					var innerName = re.exec(name);
					
					if ( innerName && innerName[1] )
					{
						name = innerName[1];
					}
				}
				
				var t = ipb.squote.parseIsoDate( $('post_id_'+post).down('.published').title )
				timestamp = t.getTime()/1000.0;
			}
			content = ( document.selection ) ? sel.text : sel.toString();
		} else {
			return alert( ipb.lang['wrong_selection'] );
		}

		// preparing content
		var quote = '[quote';
		if ( name ) { quote = quote + ' name=\'' + name + '\''; }
		if ( timestamp ) { quote = quote + ' timestamp=\'' + timestamp + '\''; }
		if ( post ) { quote = quote + ' post=\'' + post + '\''; }
		quote = quote + ']\n' + content + '\n[/quote]';

		if ( ipb.topic.fastReplyId )
		{
			var editor = ipb.textEditor.getEditor( ipb.topic.fastReplyId );
			if ( !editor.options.bypassCKEditor )
			{
				editor.insert( quote.replace( /(\r\n|\n|\r)/gm, '<br />') );
			} else {
				editor.insert( quote );
			}
		}
	}
};

ipb.squote.init();
