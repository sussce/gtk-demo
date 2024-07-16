static void
gtk_flow_box_size_allocate (GtkWidget *widget,
                            int        width,
                            int        height,
                            int        baseline)
{
  GtkFlowBox *box = GTK_FLOW_BOX (widget);
  GtkFlowBoxPrivate  *priv = BOX_PRIV (box);
  GtkAllocation child_allocation;
  int avail_size, avail_other_size, min_items, item_spacing, line_spacing;
  GtkAlign item_align;
  GtkAlign line_align;
  GtkRequestedSize *line_sizes = NULL;
  GtkRequestedSize *item_sizes = NULL;
  int min_item_size, nat_item_size;
  int line_length;
  int item_size = 0;
  int line_size = 0, min_fixed_line_size = 0, nat_fixed_line_size = 0;
  int line_offset, item_offset, n_children, n_lines, line_count;
  int extra_pixels = 0, extra_per_item = 0, extra_extra = 0;
  int extra_line_pixels = 0, extra_per_line = 0, extra_line_extra = 0;
  int i, this_line_size;
  GSequenceIter *iter;

  min_items = MAX (1, priv->min_children_per_line);

  if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      avail_size = width;
      avail_other_size = height;
      item_spacing = priv->column_spacing; line_spacing = priv->row_spacing;
    }
  else /* GTK_ORIENTATION_VERTICAL */
    {
      avail_size = height;
      avail_other_size = width;
      item_spacing = priv->row_spacing;
      line_spacing = priv->column_spacing;
    }

  item_align = ORIENTATION_ALIGN (box);
  line_align = OPPOSING_ORIENTATION_ALIGN (box);

  /* Get how many lines we'll be needing to flow */
  n_children = get_visible_children (box);

  if (n_children <= 0)
    return;

  /* Deal with ALIGNED/HOMOGENEOUS modes first, start with
   * initial guesses at item/line sizes
   */
  get_max_item_size (box, priv->orientation, &min_item_size, 
                     &nat_item_size);

  if (nat_item_size <= 0)
    {
      child_allocation.x = 0;
      child_allocation.y = 0;
      child_allocation.width = 0;
      child_allocation.height = 0;
      for (iter = g_sequence_get_begin_iter (
                                             priv->children);
           !g_sequence_iter_is_end (iter);
           iter = g_sequence_iter_next (iter))
        {
          GtkWidget *child;
          child = g_sequence_get (iter);
          if (!child_is_visible (child))
            continue;
          gtk_widget_size_allocate (child, &child_allocation, 
                                    -1);
        }
      return;
    }

  /* By default flow at the natural item width */
  line_length = avail_size / (nat_item_size + item_spacing);

  /* After the above approximation, check if we can't fit one more on the line */
  if (line_length * item_spacing + (line_length + 1) * nat_item_size <= avail_size)
    line_length++;

  /* Its possible we were allocated just less than the natural width of the
   * minimum item flow length
   */
  line_length = MAX (min_items, line_length);
  line_length = MIN (line_length, priv->max_children_per_line);

  /* Here we just use the largest height-for-width and use that for the height
   * of all lines
   */
  if (priv->homogeneous)
    {
      n_lines = n_children / line_length;
      if ((n_children % line_length) > 0)
        n_lines++;
      n_lines = MAX (n_lines, 1);
      /* Now we need the real item allocation size */
      item_size = (avail_size - (line_length - 1) * item_spacing) / line_length;
      /* Cut out the expand space if we're not distributing any */
      if (item_align != GTK_ALIGN_FILL)
        item_size = MIN (item_size, nat_item_size);
      get_largest_size_for_opposing_orientation (box,
                                                 
                                                 priv->orientation,
                                                 item_size,
                                                 
                                                 &min_fixed_line_size,
                                                 
                                                 &nat_fixed_line_size);
      /* resolve a fixed 'line_size' */
      line_size = (avail_other_size - (n_lines - 1) * line_spacing) / n_lines;
      if (line_align != GTK_ALIGN_FILL)
        line_size = MIN (line_size, nat_fixed_line_size);
      /* Get the real extra pixels in case of GTK_ALIGN_START lines */
      extra_pixels = avail_size - (line_length - 1) * item_spacing - item_size * line_length;
      extra_line_pixels = avail_other_size - (n_lines - 1) * line_spacing - line_size * n_lines;
    }
  else
    {
      gboolean first_line = TRUE;
      /* Find the amount of columns that can fit aligned into the available space
       * and collect their requests.
       */
      item_sizes = fit_aligned_item_requests (box,
                                              
                                              priv->orientation,
                                              avail_size,
                                              item_spacing,
                                              
                                              &line_length,
                                              
                                              priv->max_children_per_line,
                                              n_children);
      /* Calculate the number of lines after determining the final line_length */
      n_lines = n_children / line_length;
      if ((n_children % line_length) > 0)
        n_lines++;
      n_lines = MAX (n_lines, 1);
      line_sizes = g_new0 (GtkRequestedSize, n_lines);
      /* Get the available remaining size */
      avail_size -= (line_length - 1) * item_spacing;
      for (i = 0; i < line_length; i++)
        avail_size -= item_sizes[i].minimum_size;
      /* Perform a natural allocation on the columnized items and get the remaining pixels */
      if (avail_size > 0)
        extra_pixels = gtk_distribute_natural_allocation (avail_size, line_length, 
                                                          item_sizes);
      /* Now that we have the size of each column of items find the size of each individual
       * line based on the aligned item sizes.
       */
      for (i = 0, iter = g_sequence_get_begin_iter (priv->children);
           !g_sequence_iter_is_end (iter) && i < n_lines;
           i++)
        {
          iter = get_largest_size_for_line_in_opposing_orientation (
                       box,
                       priv->orientation,
                       iter,
                       line_length,
                       item_sizes,
                       extra_pixels,
                       &line_sizes[i].minimum_size,
                       &line_sizes[i].natural_size
          );

          /* Its possible a line is made of completely invisible children */
          if (line_sizes[i].natural_size > 0)
            {
              if (first_line)
                first_line = FALSE;
              else
                avail_other_size -= line_spacing;
              avail_other_size -= line_sizes[i].minimum_size;
              line_sizes[i].data = GINT_TO_POINTER (i);
            }
        }
      /* Distribute space among lines naturally */
      if (avail_other_size > 0)
        extra_line_pixels = gtk_distribute_natural_allocation (
                     avail_other_size,
                     n_lines,
                     line_sizes
        );
    }

  /*
   * Initial sizes of items/lines guessed at this point,
   * go on to distribute expand space if needed.
   */
  priv->cur_children_per_line = line_length;

  /* FIXME: This portion needs to consider which columns
   * and rows asked for expand space and distribute those
   * accordingly for the case of ALIGNED allocation.
   *
   * If at least one child in a column/row asked for expand;
   * we should make that row/column expand entirely.
   */
  /* Calculate expand space per item */
  if (item_align == GTK_ALIGN_FILL)
    {
      extra_per_item = extra_pixels / line_length;
      extra_extra    = extra_pixels % line_length;
    }

  /* Calculate expand space per line */
  if (line_align == GTK_ALIGN_FILL)
    {
      extra_per_line   = extra_line_pixels / n_lines;
      extra_line_extra = extra_line_pixels % n_lines;
    }

  /* prepend extra space to item_offset/line_offset for SPREAD_END */
  item_offset = get_offset_pixels (item_align, 
                                   extra_pixels);
  line_offset = get_offset_pixels (line_align, 
                                   extra_line_pixels);

  /* Get the allocation size for the first line */
  if (priv->homogeneous)
    this_line_size = line_size;
  else
    {
      this_line_size = line_sizes[0].minimum_size;
      if (line_align == GTK_ALIGN_FILL)
        {
          this_line_size += extra_per_line;
          if (extra_line_extra > 0)
            this_line_size++;
        }
    }

  i = 0; line_count = 0;
  for (iter = g_sequence_get_begin_iter (priv->children);
       !g_sequence_iter_is_end (iter);
       iter = g_sequence_iter_next (iter))
    {
      GtkWidget *child;
      int position;
      int this_item_size;

      child = g_sequence_get (iter);

      if (!child_is_visible (child))
        continue;

      /* Get item position */
      position = i % line_length;

      /* adjust the line_offset/count at the beginning of each new line */
      if (i > 0 && position == 0)
        {
          /* Push the line_offset */
          line_offset += this_line_size + line_spacing;
          line_count++;

          /* Get the new line size */
          if (priv->homogeneous)
            this_line_size = line_size;
          else
            {
              this_line_size = line_sizes[line_count].minimum_size;
              if (line_align == GTK_ALIGN_FILL)
                {
                  this_line_size += extra_per_line;
                  if (line_count < extra_line_extra)
                    this_line_size++;
                }
            }

          item_offset = 0;
          if (item_align == GTK_ALIGN_CENTER)
            {
              item_offset += get_offset_pixels (item_align, extra_pixels);
            }
          else if (item_align == GTK_ALIGN_END)
            {
              item_offset += get_offset_pixels (item_align, extra_pixels);

              /* If we're on the last line, prepend the space for
               * any leading items */
              if (line_count == n_lines -1)
                {
                  int extra_items = n_children % line_length;
                  if (priv->homogeneous)
                    {
                      item_offset += item_size * (line_length - extra_items);
                      item_offset += item_spacing * (line_length - extra_items);
                    }
                  else
                    {
                      int j;
                      for (j = 0; j < (line_length - extra_items); j++)
                        {
                          item_offset += item_sizes[j].minimum_size;
                          item_offset += item_spacing;
                        }
                    }
                }
            }
        }

      /* Push the index along for the last line when spreading to the end */
      if (item_align == GTK_ALIGN_END && line_count == n_lines -1)
        {
          int extra_items = n_children % line_length;
          position += line_length - extra_items;
        }

      if (priv->homogeneous)
        this_item_size = item_size;
      else
        this_item_size = item_sizes[position].minimum_size;

      if (item_align == GTK_ALIGN_FILL)
        {
          this_item_size += extra_per_item;
          if (position < extra_extra)
            this_item_size++;
        }

      /* Do the actual allocation */
      if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
        {
          child_allocation.x = item_offset;
          child_allocation.y = line_offset;
          child_allocation.width = this_item_size;
          child_allocation.height = this_line_size;
        }
      else /* GTK_ORIENTATION_VERTICAL */
        {
          child_allocation.x = line_offset;
          child_allocation.y = item_offset;
          child_allocation.width = this_line_size;
          child_allocation.height = this_item_size;
        }

      if (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)
        child_allocation.x = width - child_allocation.x - child_allocation.width;

      gtk_widget_size_allocate (child, &child_allocation, -1);

      item_offset += this_item_size;
      item_offset += item_spacing;
      i++;
    }
  
  g_free (item_sizes);
  g_free (line_sizes);
}

static void
gtk_flow_box_measure (GtkWidget *widget, GtkOrientation orientation, int for_size,
                      int *minimum, int *natural,
                      int *minimum_baseline, int *natural_baseline)
{
  GtkFlowBox *box = GTK_FLOW_BOX (widget);
  GtkFlowBoxPrivate *priv = BOX_PRIV (box);
  
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      if (for_size < 0)
        {
          int min_item_width, nat_item_width;
          int min_items, nat_items;
          int min_width, nat_width;
          
          min_items = MAX (1, priv->min_children_per_line);
          nat_items = MAX (min_items, priv->max_children_per_line);
          
          if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
            {
              min_width = nat_width = 0;
              
              if (!priv->homogeneous)
                {
                  /* When not homogeneous; horizontally oriented boxes
                   * need enough width for the widest row
                   */
                  if (min_items == 1)
                    {
                      get_max_item_size (box, GTK_ORIENTATION_HORIZONTAL,
                                         &min_item_width, &nat_item_width);
                      
                      min_width += min_item_width;
                      nat_width += nat_item_width;
                    }
                  else
                    {/* for_size < 0, self->orientation == orientation*/
                      int min_line_length, nat_line_length;
                      get_largest_aligned_line_length (box, GTK_ORIENTATION_HORIZONTAL,
                                                       min_items,
                                                       &min_line_length, &nat_line_length);
                      
                      if (nat_items > min_items)
                        get_largest_aligned_line_length (box, GTK_ORIENTATION_HORIZONTAL,
                                                         nat_items,
                                                         NULL, &nat_line_length);
                      
                      min_width += min_line_length;
                      nat_width += nat_line_length;
                    }
                }
              else /* In homogeneous mode; horizontally oriented boxes
                    * give the same width to all children */
                {
                  get_max_item_size (box,
                                     GTK_ORIENTATION_HORIZONTAL,
                                     &min_item_width,
                                     &nat_item_width);
                  min_width += min_item_width * min_items;
                  min_width += (min_items -1) * priv->column_spacing;
                  nat_width += nat_item_width * nat_items;
                  nat_width += (nat_items -1) * priv->column_spacing;
                }
            }
          else /* GTK_ORIENTATION_VERTICAL */
            {
              /* Return the width for the minimum height */
              int min_height;
              int dummy;
              gtk_flow_box_measure (widget, GTK_ORIENTATION_VERTICAL, -1,
                                    &min_height, &dummy,
                                    NULL, NULL);
              
              gtk_flow_box_measure (widget, GTK_ORIENTATION_HORIZONTAL, min_height,
                                    &min_width, &nat_width,
                                    NULL, NULL);
            }
          
          *minimum = min_width;
          *natural = nat_width;
        }
      else
        {
          int min_item_height, nat_item_height;
          int min_items;
          int min_width, nat_width;
          int avail_size, n_children;
          
          min_items = MAX (1, priv->min_children_per_line);
          min_width = 0;
          nat_width = 0;
          
          if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
            {
              /* Return the minimum width */
              gtk_flow_box_measure (widget, GTK_ORIENTATION_HORIZONTAL, -1,
                                    &min_width, &nat_width,
                                    NULL, NULL);
            }
          else /* GTK_ORIENTATION_VERTICAL */
            {
              int min_height;
              int line_length;
              int item_size, extra_pixels;
              int dummy;
              
              n_children = get_visible_children (box);
              
              if (n_children <= 0) {
                goto out_width;
              }
              
              /* Make sure its no smaller than the minimum */
              gtk_flow_box_measure (widget, GTK_ORIENTATION_VERTICAL, -1,
                                    &min_height, &dummy, NULL, NULL);
              
              avail_size = MAX (for_size, min_height);
              
              if (avail_size <= 0) {
                goto out_width;
              }
              
              get_max_item_size (box, GTK_ORIENTATION_VERTICAL,
                                 &min_item_height, &nat_item_height);
              
              if (nat_item_height <= 0) {
                goto out_width;
              }
              
              /* By default flow at the natural item width */
              line_length = avail_size / (nat_item_height + priv->row_spacing);

              /* After the above approximation, check if we can't fit one more on the line */
              if (line_length * priv->row_spacing +
                  (line_length + 1) * nat_item_height <= avail_size) {
                line_length++;
              }

              /* Its possible we were allocated just less than the natural width of the
               * minimum item flow length
               */
              line_length = MAX (min_items, line_length);
              line_length = MIN (line_length, priv->max_children_per_line);

              /* Now we need the real item allocation size */
              item_size = (avail_size - (line_length - 1) * priv->row_spacing) / line_length;

              /* Cut out the expand space if we're not distributing any */
              if (gtk_widget_get_valign (widget) != GTK_ALIGN_FILL)
                {
                  item_size = MIN (item_size, nat_item_height);
                  extra_pixels = 0;
                }
              else
                {
                /* Collect the extra pixels for expand children */
                extra_pixels = (avail_size - (line_length - 1) * priv->row_spacing) % line_length;
              }
              
              if (priv->homogeneous)
                {
                  int min_item_width, nat_item_width;
                  int lines;
                  /* Here we just use the largest height-for-width and
                   * add up the size accordingly
                   */
                  get_largest_size_for_opposing_orientation (box,
                                                             
                                                             GTK_ORIENTATION_VERTICAL,
                                                             item_size,
                                                             
                                                             &min_item_width,
                                                             
                                                             &nat_item_width);
                  /* Round up how many lines we need to allocate for */
                  n_children = get_visible_children (box);
                  lines = n_children / line_length;
                  if ((n_children % line_length) > 0)
                    lines++;
                  min_width = min_item_width * lines;
                  nat_width = nat_item_width * lines;
                  min_width += (lines - 1) * priv->column_spacing;
                  nat_width += (lines - 1) * priv->column_spacing;
                }
              else
                {
                  int min_line_width, nat_line_width, i;
                  gboolean first_line = TRUE;
                  GtkRequestedSize *item_sizes;
                  GSequenceIter *iter;
                  
                  /* First get the size each set of items take to span the line
                   * when aligning the items above and below after flowping.
                   */
                  item_sizes =
                    fit_aligned_item_requests (box, priv->orientation,
                                               avail_size,
                                               priv->row_spacing,
                                               &line_length,
                                               priv->max_children_per_line,
                                               n_children);
                  
                  /* Get the available remaining size */
                  avail_size -= (line_length - 1) * priv->column_spacing;

                  for (i = 0; i < line_length; i++) {
                    avail_size -= item_sizes[i].minimum_size;
                  }

                  if (avail_size > 0) {
                    extra_pixels =
                      gtk_distribute_natural_allocation (avail_size,
                                                         line_length,
                                                         item_sizes);
                  }
                  
                  for (iter = g_sequence_get_begin_iter (priv->children);
                       !g_sequence_iter_is_end (iter);)
                    {
                      iter =
                        get_largest_size_for_line_in_opposing_orientation (
                            box,
                            GTK_ORIENTATION_VERTICAL,
                            iter,
                            line_length,
                            item_sizes,
                            extra_pixels,
                            &min_line_width,
                            &nat_line_width
                         );
                      
                      /* Its possible the last line only had invisible widgets */
                      if (nat_line_width > 0)
                        {
                          if (first_line)
                            first_line = FALSE;
                          else
                            {
                              min_width += priv->column_spacing;
                              nat_width += priv->column_spacing;
                            }
                          
                          min_width += min_line_width;
                          nat_width += nat_line_width;
                        }
                    }

                  g_free (item_sizes);
                }
            }
          
        out_width:
          *minimum = min_width;
          *natural = nat_width;
        }
    }
  else
    {
      if (for_size < 0)
        {
          int min_item_height, nat_item_height;
          int min_items, nat_items;
          int min_height, nat_height;

          min_items = MAX (1, priv->min_children_per_line);
          nat_items = MAX (min_items, priv->max_children_per_line);

          if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
            {
              /* Return the height for the minimum width */
              int min_width;
              int dummy;
              
              gtk_flow_box_measure (widget, GTK_ORIENTATION_HORIZONTAL, -1,
                                    &min_width, &dummy, NULL, NULL);
              
              gtk_flow_box_measure (widget, GTK_ORIENTATION_VERTICAL, min_width,
                                    &min_height, &nat_height, NULL, NULL);
            }
          else /* GTK_ORIENTATION_VERTICAL */
            {
              min_height = nat_height = 0;
              
              if (! priv->homogeneous)
                {
                  /* When not homogeneous; vertically oriented boxes
                   * need enough height for the tallest column
                   */
                  if (min_items == 1)
                    {
                      get_max_item_size (box,
                                         GTK_ORIENTATION_VERTICAL,
                                         &min_item_height,
                                         &nat_item_height);
                      
                      min_height += min_item_height;
                      nat_height += nat_item_height;
                    }
                  else
                    {
                      int min_line_length, nat_line_length;
                      
                      get_largest_aligned_line_length (box, GTK_ORIENTATION_VERTICAL,
                                                       min_items,
                                                       &min_line_length, &nat_line_length);
                      
                      if (nat_items > min_items)
                        get_largest_aligned_line_length (box, GTK_ORIENTATION_VERTICAL,
                                                         nat_items,
                                                         NULL, &nat_line_length);
                      
                      min_height += min_line_length;
                      nat_height += nat_line_length;
                    }
                }
              else
                {
                  /* In homogeneous mode; vertically oriented boxes
                   * give the same height to all children
                   */
                  get_max_item_size (box,
                                     
                                     GTK_ORIENTATION_VERTICAL,
                                     
                                     &min_item_height,
                                     
                                     &nat_item_height);
                  min_height += min_item_height * min_items;
                  min_height += (min_items -1) * priv->row_spacing;
                  nat_height += nat_item_height * nat_items;
                  nat_height += (nat_items -1) * priv->row_spacing;
                }
            }

          *minimum = min_height;
          *natural = nat_height;
        }
      else
        {
          int min_item_width, nat_item_width;
          int min_items;
          int min_height, nat_height;
          int avail_size, n_children;

          min_items = MAX (1, priv->min_children_per_line);
          min_height = 0;
          nat_height = 0;

          if (priv->orientation == GTK_ORIENTATION_HORIZONTAL)
            {/* self->orientation != orientation, for_size > 0 */
              int min_width;
              int line_length;
              int item_size, extra_pixels;
              int dummy;
              
              n_children = get_visible_children (box);
              
              if (n_children <= 0)
                goto out_height;
              
              /* Make sure its no smaller than the minimum */
              gtk_flow_box_measure (widget, GTK_ORIENTATION_HORIZONTAL, -1,
                                    &min_width, &dummy, NULL, NULL);

              avail_size = MAX (for_size, min_width);

              if (avail_size <= 0)
                goto out_height;

              get_max_item_size (box, GTK_ORIENTATION_HORIZONTAL, &min_item_width,
                                 &nat_item_width);

              if (nat_item_width <= 0)
                goto out_height;

              /* By default flow at the natural item width */
              line_length = avail_size / (nat_item_width + priv->column_spacing);

              /* After the above approximation, check if we can't fit one more on the line */
              if (line_length * priv->column_spacing + (line_length + 1) * nat_item_width <= avail_size)
                line_length++;

              /* Its possible we were allocated just less than the natural width of the
               * minimum item flow length
               */
              line_length = MAX (min_items, line_length);
              line_length = MIN (line_length, priv->max_children_per_line);

              /* Now we need the real item allocation size */
              item_size = (avail_size - (line_length - 1) * priv->column_spacing) / line_length;

              /* Cut out the expand space if we're not distributing any */
              if (gtk_widget_get_halign (widget) != GTK_ALIGN_FILL)
                {
                  item_size = MIN (item_size, nat_item_width);
                  extra_pixels = 0;
                }
              else {
                /* Collect the extra pixels for expand children */
                extra_pixels = (avail_size - (line_length - 1) * priv->column_spacing) % line_length;
              }
              
              if (priv->homogeneous)
                {
                  int min_item_height, nat_item_height;
                  int lines;
                  /* Here we just use the largest height-for-width and
                   * add up the size accordingly
                   */
                  get_largest_size_for_opposing_orientation (box, GTK_ORIENTATION_HORIZONTAL, item_size,
                                                             &min_item_height, &nat_item_height);

                  /* Round up how many lines we need to allocate for */
                  lines = n_children / line_length;

                  if ((n_children % line_length) > 0)
                    lines++;
                  
                  min_height = min_item_height * lines;
                  nat_height = nat_item_height * lines;
                  min_height += (lines - 1) * priv->row_spacing;
                  nat_height += (lines - 1) * priv->row_spacing;
                }
              else
                {
                  int min_line_height, nat_line_height, i;
                  gboolean first_line = TRUE;
                  GtkRequestedSize *item_sizes;
                  GSequenceIter *iter;

                  /* First get the size each set of items take to span the line
                   * when aligning the items above and below after flowping.
                   */
                  item_sizes = fit_aligned_item_requests (box,
                                                          priv->orientation,
                                                          avail_size,
                                                          priv->column_spacing,
                                                          &line_length,
                                                          priv->max_children_per_line,
                                                          n_children);

                  /* Get the available remaining size */
                  avail_size -= (line_length - 1) * priv->column_spacing;

                  for (i = 0; i < line_length; i++)
                    avail_size -= item_sizes[i].minimum_size;

                  if (avail_size > 0) {
                    extra_pixels = gtk_distribute_natural_allocation (avail_size,
                                                                      line_length,
                                                                      item_sizes);
                  }
                  
                  for (iter = g_sequence_get_begin_iter (priv->children);
                       !g_sequence_iter_is_end (iter);)
                    {
                      iter = get_largest_size_for_line_in_opposing_orientation (
                                 box,
                                 GTK_ORIENTATION_HORIZONTAL,
                                 iter,
                                 line_length,
                                 item_sizes,
                                 extra_pixels,
                                 &min_line_height,
                                 &nat_line_height
                              );
                      
                      /* Its possible the line only had invisible widgets */
                      if (nat_line_height > 0)
                        {
                          if (first_line)
                            first_line = FALSE;
                          else
                            {
                              min_height += priv->row_spacing;
                              nat_height += priv->row_spacing;
                            }
                          min_height += min_line_height;
                          nat_height += nat_line_height;
                        }
                    }
                  
                  g_free (item_sizes);
                }
            }
          else /* GTK_ORIENTATION_VERTICAL */
            {
              /* Return the minimum height */
              gtk_flow_box_measure (widget, GTK_ORIENTATION_VERTICAL, -1,
                                    &min_height, &nat_height, NULL, NULL);
            }

        out_height:
          *minimum = min_height;
          *natural = nat_height;
        }
    }
}

static void
get_largest_aligned_line_length (GtkFlowBox *box, GtkOrientation  orientation,
                                 int line_length, int *min_size, int *nat_size)
{
  GSequenceIter *iter;
  int max_min_size = 0;
  int max_nat_size = 0;
  int spacing, i;
  GtkRequestedSize *aligned_item_sizes;
  
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    spacing = BOX_PRIV (box)->column_spacing;
  else
    spacing = BOX_PRIV (box)->row_spacing;
  aligned_item_sizes = g_new0 (GtkRequestedSize, line_length);

  /* Get the largest sizes of each index in the line.
   */
  i = 0;
  for (iter = g_sequence_get_begin_iter (BOX_PRIV (box)->children);
       !g_sequence_iter_is_end (iter);
       iter = g_sequence_iter_next (iter))
    {
      GtkWidget *child;
      int child_min, child_nat;
      
      child = g_sequence_get (iter);
      if (!child_is_visible (child))
        continue;

      gtk_widget_measure (child, orientation, -1,
                          &child_min, &child_nat, NULL, NULL);
      
      aligned_item_sizes[i % line_length].minimum_size =
        MAX (aligned_item_sizes[i % line_length].minimum_size, child_min);
      aligned_item_sizes[i % line_length].natural_size =
        MAX (aligned_item_sizes[i % line_length].natural_size, child_nat);
      i++;
    }
  
  /* Add up the largest indexes */
  for (i = 0; i < line_length; i++)
    {
      max_min_size += aligned_item_sizes[i].minimum_size;
      max_nat_size += aligned_item_sizes[i].natural_size;
    }

  g_free (aligned_item_sizes);

  max_min_size += (line_length - 1) * spacing;
  max_nat_size += (line_length - 1) * spacing;

  if (min_size)
    *min_size = max_min_size;
  if (nat_size)
    *nat_size = max_nat_size;
}

static GSequenceIter *
get_largest_size_for_line_in_opposing_orientation (GtkFlowBox       *box,
                                                   GtkOrientation    orientation,
                                                   GSequenceIter    *cursor,
                                                   int               line_length,
                                                   GtkRequestedSize *item_sizes,
                                                   int               extra_pixels,
                                                   int              *min_item_size,
                                                   int              *nat_item_size)
{
  GSequenceIter *iter;
  int max_min_size = 0;
  int max_nat_size = 0;
  int i;
  
  i = 0;
  for (iter = cursor;
       !g_sequence_iter_is_end (iter) && i < line_length;
       iter = g_sequence_iter_next (iter))
    {
      GtkWidget *child;
      int child_min, child_nat, this_item_size;

      child = g_sequence_get (iter);
      if (!child_is_visible (child))
        continue;
      
      /* Distribute the extra pixels to the first children in the line
       * (could be fancier and spread them out more evenly) */
      this_item_size = item_sizes[i].minimum_size;

      if (extra_pixels > 0 && ORIENTATION_ALIGN (box) == GTK_ALIGN_FILL)
        {
          this_item_size++;
          extra_pixels--;
        }
      
      gtk_widget_measure (child, 1 - orientation, this_item_size,
                          &child_min, &child_nat, NULL, NULL);

      max_min_size = MAX (max_min_size, child_min);
      max_nat_size = MAX (max_nat_size, child_nat);
      i++;
    }

  if (min_item_size)
    *min_item_size = max_min_size;
  if (nat_item_size)
    *nat_item_size = max_nat_size;

  /* Return next item in the list */
  return iter;
}
